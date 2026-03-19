import numpy as np
import wave

# Parameters
sample_rate = 16000  # 16kHz
duration = 2  # 2 seconds
amplitude = 32767  # 16-bit PCM
frequency = 440  # 440Hz (A4)
noise_level = 0.5  # Noise level (0-1)

# Generate time array
t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)

# Generate sine wave
sine = np.sin(2 * np.pi * frequency * t)

# Generate noise
noise = noise_level * np.random.randn(len(t))

# Combine sine and noise
signal = sine + noise

# Normalize to 16-bit range
signal = np.clip(signal, -1, 1)
signal = (signal * amplitude).astype(np.int16)

# Write to WAV file
with wave.open('test_input.wav', 'w') as wf:
    wf.setnchannels(1)  # Mono
    wf.setsampwidth(2)  # 16-bit
    wf.setframerate(sample_rate)
    wf.writeframes(signal.tobytes())

print(f"Generated test_input.wav: {duration} seconds, {sample_rate} Hz, 16-bit PCM")
print(f"Contains 440Hz sine wave + noise (level: {noise_level})")