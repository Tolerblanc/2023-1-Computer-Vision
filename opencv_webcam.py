import cv2

cap = cv2.VideoCapture(1)

if not cap.isOpened():
	raise IOError("Cannot open webcam")

while True:
	ret, frame = cap.read()
	if (ret):
		cv2.imshow('webcam', frame)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break
cap.release()
cv2.destroyAllWindows()