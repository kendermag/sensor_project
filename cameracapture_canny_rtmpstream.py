import numpy as np
import cv2
from multiprocessing import Process

## the rtmp key goes where the x-es are 

## TBA TODO: 

## removing UI from jetson would greatly enchant inference speed at higher resolutions, while decresaing battery usage

def send():
    cap_send = cv2.VideoCapture("nvarguscamerasrc \
        ! video/x-raw(memory:NVMM), format=(string)NV12, width=(int)640, height=(int)480, framerate=(fraction)24/1 \
        ! nvvidconv ! video/x-raw, format=(string)RGBA ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", cv2.CAP_GSTREAMER)
    
    
    out_send = cv2.VideoWriter('appsrc ! videoconvert ! nvvidconv \
        ! nvv4l2h264enc \
        ! h264parse \
        ! queue \
        ! flvmux streamable=true name=mux \
        ! rtmpsink location="rtmp://a.rtmp.youtube.com/live2/xxxx-xxxx-xxxx-xxxx-xxxx app=live2" \
        audiotestsrc \
        ! voaacenc bitrate=432 \
        ! mux.',cv2.CAP_GSTREAMER,0, 24, (640,480), True)

    if not cap_send.isOpened() or not out_send.isOpened():
        print('VideoCapture or VideoWriter not opened')
        exit(0)

    while True:
        ret,frame = cap_send.read()

        if not ret:
            print('empty frame')
            break
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray, (7, 7), 0)
        canny = cv2.Canny(blur, 15, 70)
        ret, mask = cv2.threshold(canny, 45, 255, cv2.THRESH_BINARY)
        stream_frame = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
        out_send.write(stream_frame)

        if cv2.waitKey(1)&0xFF == ord('q'):
            break

    cap_send.release()
    out_send.release()

if __name__ == '__main__':
    send()

    cv2.destroyAllWindows()
