import cv2
import cv2.cv as cv

v = cv2.VideoCapture()

v.open("/home/minde/Desktop/a.wmv")

while (v.isOpened()):
    retv, frame = v.read()
    d = cv.fromarray(frame)
    cv.ShowImage("video", d)
    if cv.WaitKey(10) == 27:
        break

cv.DestroyAllWindows()
