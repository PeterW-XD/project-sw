import serial
import time

# Open the serial port
ser = serial.Serial('COM3', 115200)  # Replace 'COM1' with the appropriate serial port

# Open a text file for writing
with open('C:\FILE\i2s_inmp441\output.txt', 'w') as file:
    start_time = time.time()
    while True:
        # Read a line from the serial port
        line = ser.readline().decode().strip()  # Decode bytes to string and strip whitespace
        
        # Split the line into individual sensor readings
        sensor_readings = line.split(' ')
        
        # Write the sensor readings to the text file
        file.write(' '.join(sensor_readings) + '\n')
        
        # Print the sensor readings to the console (optional)
        print(sensor_readings)

        if time.time() - start_time >= 3:
            break