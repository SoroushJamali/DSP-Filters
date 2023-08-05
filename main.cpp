// Including necessary libraries
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

// Function for low-pass filtering
std::vector<double> lowPassFilter(const std::vector<double> &signal, double cutoffFrequency, double dt, double &alpha) {
    // Initialising the output signal vector
    std::vector<double> filteredSignal(signal.size(), 0.0);
    
    // Calculating RC (Resistor-Capacitor) constant
    double RC = 1.0 / (cutoffFrequency * 2 * M_PI);
    
    // Calculating alpha (used in exponential smoothing)
    alpha = dt / (RC + dt);

    // Setting the first element
    filteredSignal[0] = signal[0];

    // Looping through the rest of the signal
    for (size_t i = 1; i < signal.size(); i++) {
        // Implementing the exponential smoothing
        filteredSignal[i] = filteredSignal[i - 1] + (alpha * (signal[i] - filteredSignal[i - 1]));
    }

    // Returning the filtered signal
    return filteredSignal;
}

// Function for high-pass filtering
std::vector<double> highPassFilter(const std::vector<double> &signal, double cutoffFrequency, double dt, double &alpha) {
    std::vector<double> filteredSignal(signal.size(), 0.0);
    double RC = 1.0 / (cutoffFrequency * 2 * M_PI);
    alpha = dt / (RC + dt);

    // The implementation is almost the same as the low-pass filter, but with a different smoothing
    filteredSignal[0] = signal[0];
    for (size_t i = 1; i < signal.size(); i++) {
        filteredSignal[i] = alpha * (filteredSignal[i - 1] + signal[i] - signal[i - 1]);
    }

    return filteredSignal;
}

// Function for band-pass filtering
std::vector<double> bandPassFilter(const std::vector<double> &signal, double cutoffFrequencyLow, double cutoffFrequencyHigh, double dt, double &alphaLow, double &alphaHigh) {
    // First, the signal is low-pass filtered
    std::vector<double> lowPass = lowPassFilter(signal, cutoffFrequencyHigh, dt, alphaHigh);
    
    // Then, the result is high-pass filtered
    std::vector<double> bandPass = highPassFilter(lowPass, cutoffFrequencyLow, dt, alphaLow);

    return bandPass;
}

// The main function
int main() {
    // Variables for the original signal frequencies
    double frequency1 = 6.0;
    double frequency2 = 10.0;

    // Sample signal
    std::vector<double> signal;
    for (int i = 0; i < 1000; i++) {
        // The original signal is a mix of two sine waves
        signal.push_back(sin(2 * M_PI * i / (100 / frequency1)) + sin(2 * M_PI * i / (100 / frequency2)));
    }

    // Defining cutoff frequencies and sampling rate
    double cutoffFrequencyLow = 8.0;
    double cutoffFrequencyHigh = 35.0;
    double samplingRate = 100.0;
    double dt = 1.0 / samplingRate;
    double alphaLow, alphaHigh;

    // Applying low-pass filter
    std::vector<double> lowPassSignal = lowPassFilter(signal, cutoffFrequencyLow, dt, alphaLow);

    // Applying high-pass filter
    std::vector<double> highPassSignal = highPassFilter(signal, cutoffFrequencyHigh, dt, alphaHigh);

    // Applying band-pass filter
    std::vector<double> bandPassSignal = bandPassFilter(signal, cutoffFrequencyLow, cutoffFrequencyHigh, dt, alphaLow, alphaHigh);

    // Saving signals to files
    std::ofstream originalSignalFile("original_signal.txt");
    std::ofstream lowPassSignalFile("lowpass_signal.txt");
    std::ofstream highPassSignalFile("highpass_signal.txt");
    std::ofstream bandPassSignalFile("bandpass_signal.txt");

    // Frequency response of filters
    std::ofstream frequencyResponseLowFile("frequency_response_low.txt");
    std::ofstream frequencyResponseHighFile("frequency_response_high.txt");
    std::ofstream frequencyResponseBandFile("frequency_response_band.txt");

    // Saving signals and frequency responses to files
    for (size_t i = 0; i < signal.size(); i++) {
        originalSignalFile << signal[i] << '\n';
        lowPassSignalFile << lowPassSignal[i] << '\n';
        highPassSignalFile << highPassSignal[i] << '\n';
        bandPassSignalFile << bandPassSignal[i] << '\n';
    }

    // Saving frequency responses to files
    for (int f = 0; f <= 50; f++) {
        double responseLow = 1 / sqrt(1 + pow((f / cutoffFrequencyLow), 2));
        double responseHigh = pow((f / cutoffFrequencyHigh), 2) / sqrt(1 + pow((f / cutoffFrequencyHigh), 2));
        double responseBand = responseHigh * responseLow;

        frequencyResponseLowFile << f << " " << responseLow << '\n';
        frequencyResponseHighFile << f << " " << responseHigh << '\n';
        frequencyResponseBandFile << f << " " << responseBand << '\n';
    }

    originalSignalFile.close();
    lowPassSignalFile.close();
    highPassSignalFile.close();
    bandPassSignalFile.close();
    frequencyResponseLowFile.close();
    frequencyResponseHighFile.close();
    frequencyResponseBandFile.close();

    // Plotting with Gnuplot
    FILE *gnuplotPipe = _popen("gnuplot -persist", "w");

    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set title 'Original and Filtered Signals (Original Signal Frequencies: %.2fHz and %.2fHz)'\n", frequency1, frequency2);
        fprintf(gnuplotPipe, "set xlabel 'Time (samples)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Amplitude'\n");
        fprintf(gnuplotPipe, "plot 'original_signal.txt' with lines, 'lowpass_signal.txt' with lines, 'highpass_signal.txt' with lines, 'bandpass_signal.txt' with lines\n");
    
        fprintf(gnuplotPipe, "set terminal wxt 1\n");
        fprintf(gnuplotPipe, "set title 'Low-pass Filter Frequency Response (Cutoff: %.2f Hz)'\n", cutoffFrequencyLow);
        fprintf(gnuplotPipe, "set xlabel 'Frequency (Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Gain'\n");
        fprintf(gnuplotPipe, "plot 'frequency_response_low.txt' using 1:2 with lines\n");
    
        fprintf(gnuplotPipe, "set terminal wxt 2\n");
        fprintf(gnuplotPipe, "set title 'High-pass Filter Frequency Response (Cutoff: %.2f Hz)'\n", cutoffFrequencyHigh);
        fprintf(gnuplotPipe, "set xlabel 'Frequency (Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Gain'\n");
        fprintf(gnuplotPipe, "plot 'frequency_response_high.txt' using 1:2 with lines\n");
    
        fprintf(gnuplotPipe, "set terminal wxt 3\n");
        fprintf(gnuplotPipe, "set title 'Band-pass Filter Frequency Response (Low Cutoff: %.2f Hz, High Cutoff: %.2f Hz)'\n", cutoffFrequencyLow, cutoffFrequencyHigh);
        fprintf(gnuplotPipe, "set xlabel 'Frequency (Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Gain'\n");
        fprintf(gnuplotPipe, "plot 'frequency_response_band.txt' using 1:2 with lines\n");
    
        fflush(gnuplotPipe);
    }

    _pclose(gnuplotPipe);

    return 0;
}
