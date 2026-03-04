/*******************************************************************************
 * File Name:   part3_examples.h
 *
 * Description: Part 3 Examples - Oscilloscope & Signal Processing
 *              Embedded C for IoT Course
 *
 * Examples:
 *   1. Waveform Generator - Square, Sine, Triangle, Sawtooth
 *   2. Noise Generator - White noise display
 *   3. Audio Waveform - I2S audio playback visualization
 *   4. Mic Visualizer - PDM microphone live display
 *   5. Oscilloscope UI - Full oscilloscope interface
 *   6. Spectrum Analyzer - FFT frequency display
 *
 ******************************************************************************/

#ifndef PART3_EXAMPLES_H
#define PART3_EXAMPLES_H

#include "lvgl/lvgl.h"

/*******************************************************************************
 * Example Function Declarations
 ******************************************************************************/

/**
 * @brief Example 1: Waveform Generator
 *
 * Display generated waveforms (Square, Sine, Triangle, Sawtooth)
 * using LVGL Chart widget. User can switch between waveform types
 * and adjust frequency.
 *
 * Widgets used: Chart, Dropdown, Slider, Label
 */
void part3_ex1_waveform_generator(void);

/**
 * @brief Example 2: Noise Generator
 *
 * Generate and display white noise on LVGL Chart.
 * Demonstrates random signal generation.
 *
 * Widgets used: Chart, Label, Button
 */
void part3_ex2_noise_generator(void);

/**
 * @brief Example 3: Audio Waveform Display
 *
 * Visualize audio playback waveform from I2S output.
 * Shows how to integrate audio data with LVGL display.
 *
 * Widgets used: Chart, Button (Play/Pause), Slider (Volume)
 */
void part3_ex3_audio_waveform(void);

/**
 * @brief Example 4: Microphone Visualizer
 *
 * Real-time microphone input visualization using PDM mic.
 * Shows live audio level and waveform.
 *
 * Widgets used: Chart, Bar (Level), Label
 */
void part3_ex4_mic_visualizer(void);

/**
 * @brief Example 5: Full Oscilloscope UI
 *
 * Complete oscilloscope interface with:
 * - Waveform display (Chart)
 * - Time/div control
 * - Volt/div control
 * - Trigger settings
 * - Measurements (Vpp, Freq, RMS)
 *
 * Widgets used: Chart, Scale, Dropdown, Slider, Label, TabView
 */
void part3_ex5_oscilloscope_ui(void);

/**
 * @brief Example 6: Spectrum Analyzer
 *
 * FFT-based spectrum analyzer showing frequency content.
 * Uses bar chart to display frequency bins.
 *
 * Widgets used: Chart (Bar), Scale, Label
 */
void part3_ex6_spectrum_analyzer(void);

#endif /* PART3_EXAMPLES_H */
