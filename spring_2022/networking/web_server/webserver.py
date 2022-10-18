# Gabriel de Sa 1001676832
import socket 
from _thread import *
import sys
# Took inspiration from https://realpython.com/python-sockets/#tcp-sockets
# Also used python docs to learn about socket. 
HOST = sys.argv[1] if len(sys.argv) > 1 else "127.0.0.1" # Set the host ip address
PORT = int(sys.argv[2]) if len(sys.argv) > 1 else 8080 # set the port. 

availableFiles = [b"index.html", b"meme.jpg"] # Create an array of available files for security reasons
movedSites = {b"index1.html": "index.html"} # Keep a dictionary of locations that have moved. 

"""parseRequest will parse the request to the server and return back the response and the body content. 
Parse request will only return a file opened as binary as the body. """
def parseRequest(request):
    parsedHTML = request.split(b'\r\n') # Split the html into an array separated by the \r\n characters
    body = None 
    print(parsedHTML) # Prints the HTML request
    requestedFile = parsedHTML[0].split(b' ')[1][1:] # Gets the requested file, assumes the request type is get. 
    print("Retrieving: ", str(requestedFile)[1:])
    if requestedFile in availableFiles: # Make sure that the file being requested is actually a file available to the users
        body = open(requestedFile, 'rb') # Try to open the file as a binary file
        if body:  # If the file is opened then set the response
            response = b"HTTP/1.0 200 OK\r\nConnection: closed\r\nContent-type: text/html\r\n"
    elif requestedFile in movedSites: # if the file is not in the available files, check to make sure it wasn't moved
        # if the files were moved then send the appropriate response and set location flag in the HTTP response
        response = "HTTP/1.0 301 Moved Permanently\r\nConnection: closed\r\nLocation: {}\r\n".format(movedSites[requestedFile]).encode("utf-8")
    else:
        # if the file was not moved and the file was not found then send the 404 response
        response = b"HTTP/1.0 404 Not Found\r\nConnection: closed\r\n"
        # set the body to be the 404.html file
        body = open("404.html", "rb")
    return response, body

"""serverSocket is a threadable function that returns with the verification code that the connection between the server and the client closed.
Responsible for dealing with the response of a socket connection. Always respond with a status code and sends a file as the body."""
def serverSocket(connection, address):
    with connection:
        while True:
            request = connection.recv(4096) # Gets the request of size 4096 bytes. 
            if not request: # If there is no request then disconnect from the client
                print("Goodbye ", address)
                connection.close()
                break
            response, body = parseRequest(request) # Parse the request and get the response and body
            print(response) # Print response HTTP 
            connection.send(response) # Send the header
            connection.send(b"\n") # Send the new line indiciating the body will be sent next 
            connection.sendfile(body) # Send the actually body binary
            body.close() # Close the body file
            return connection.close() # Close the connection
    return # Return if connection fails randomly 

"""Main runs the webserver loop. Whenever a new request comes it starts a new thread running serverSocket"""
def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sockt: # Create the TCP Socket
        sockt.bind((HOST, PORT)) # Bind the socket to the host and ip address 
        print("Server listening at {}:{}".format(HOST, PORT))
        sockt.listen(10) # Listen to up to 10 connections
        while True:  # Keep the server running
            connection, address = sockt.accept() # if a connection request comes in accept it
            print("Connected to:", address) 
            start_new_thread(serverSocket, (connection, address, )) # Process the request
        s.close()

main()