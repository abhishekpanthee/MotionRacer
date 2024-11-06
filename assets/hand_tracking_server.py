import cv2
import HandTrackingModule as htm
import socket

# Set up the socket
host = '192.168.1.42'  # Your local IP address
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

        # Check if each finger (except thumb) is down
        for id in range(1, 5):
            if lmList[tipIds[id] - 3][2] < lmList[tipIds[id]][2]:  # Checks if tip is below its base
                fingersDown.append(1)
        
        # Determine hand gesture based on thumb orientation and other fingers
        if fingersDown.count(1) == 4:  # Only thumb should be up for directional gestures
            thumb_tip_x = lmList[tipIds[0]][1]
            thumb_tip_y = lmList[tipIds[0]][2]
            wrist_x = lmList[0][1]
            wrist_y = lmList[0][2]

            # Right
            if thumb_tip_x < wrist_x and abs(thumb_tip_x - wrist_x) > abs(thumb_tip_y - wrist_y):
                result = "Pointing Right"
            # Left
            elif thumb_tip_x > wrist_x and abs(thumb_tip_x - wrist_x) > abs(thumb_tip_y - wrist_y):
                result = "Pointing Left"
            # Up
            elif thumb_tip_y < wrist_y and abs(thumb_tip_y - wrist_y) > abs(thumb_tip_x - wrist_x):
                result = "Pointing Up"
            # Down
            elif thumb_tip_y > wrist_y and abs(thumb_tip_y - wrist_y) > abs(thumb_tip_x - wrist_x):
                result = "Pointing Down"
        
        elif fingersDown.count(1) == 0:
            result = "five"  # Open hand
        elif fingersDown.count(1) == 4:
            result = "fist"  # Closed fist

        print(result)
    
    # Send result to C++ program
    conn.sendall(result.encode())
    
    # Show the image
    cv2.imshow('Image', img)
    
    # Break loop if 'q' is pressed in the Python script window
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources and close connections
cap.release()
conn.close()
server_socket.close()
cv2.destroyAllWindows()
