import cv2
import HandTrackingModule as htm
import socket

# Set up the socket
host = '192.168.1.8'  # Your local IP address
port = 65432  # Port number

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))

server_socket.listen()


print("Waiting for a connection...")
conn, addr = server_socket.accept()
print(f"Connected by {addr}")

wCam, hCam = 640, 480
cap = cv2.VideoCapture(0)
cap.set(3, wCam)
cap.set(4, hCam)

detector = htm.handDetector(detectionCon=0.75)
tipIds = [4, 8, 12, 16, 20]

while True:
    success, img = cap.read()
    img = detector.findHands(img)
    lmList = detector.findPosition(img, draw=False)
    
    result = "unknown"  # Default value
    if len(lmList) != 0:
        fingersDown = []
        
        for id in range(1, 5):
            if lmList[tipIds[id] - 3][2] < lmList[tipIds[id]][2]:
                fingersDown.append(1)
        
        if fingersDown.count(1) == 0:
            result = "five"
        elif lmList[0][1] > lmList[tipIds[0]][1] and fingersDown.count(1) == 4 and lmList[tipIds[0] - 2][1] > lmList[tipIds[0]][1]:
            result = "Pointing Right"
        elif lmList[0][1] < lmList[tipIds[0]][1] and fingersDown.count(1) == 4 and lmList[tipIds[0] - 2][1] < lmList[tipIds[0]][1]:
            result = "Pointing Left"
        elif fingersDown.count(1) == 4:
            result = "fist"
        print(result)
    
    # Send result to C++ program
    conn.sendall(result.encode())
    
    cv2.imshow('Image', img)
    # Break loop if 'q' is pressed in the Python script window
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


cap.release()
conn.close()
server_socket.close()
cv2.destroyAllWindows()
