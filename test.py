
############## Claim ox
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import sys
import datetime

serial_port = 'COM6';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "output.txt";

output_file = open(write_to_file_path, "w");
ser = serial.Serial(serial_port, baud_rate)
while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    output_file.write(line)
    if "*" in line:
        currentDT = datetime.datetime.now()
        print("Date & Time of accident: " + str(currentDT) )
        print("\n------------------ End of Report ------------------" )
        output_file.close()
        break

        

