# Use an official MySQL image as the base image
FROM mysql:latest

# Install g++ and MySQL development files
RUN sudo apt-get update && sudo apt-get install -y g++ libmysqlclient-dev

# Set the working directory
WORKDIR /app

# Copy the C++ source code
COPY *.cpp /app/

# Compile the C++ source code
RUN g++ -o grading_app *.cpp -lmysqlcppconn

# Copy the input CSV file
COPY InputFile.csv /app/

# Define the command to run the application
CMD ["./grading_app"]
