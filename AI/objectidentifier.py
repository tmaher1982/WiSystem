import cv2
import matplotlib.pyplot as plt
import cvlib as cv
import urllib.request
import numpy as np
from cvlib.object_detection import draw_bbox
import concurrent.futures
 
import webbrowser




# Change this URL based on the ip address granted to the ESP32 cam webserver 

url='http://192.168.0.130/capture.jpg'
alert_url = 'http://192.168.0.187/setWildStatus?s=1'
safe_url = 'http://192.168.0.187/setWildStatus?s=0'
animal_detected = " "

im=None


# Based on the case call the alert System
def setAlert(animal_detected):
     
    print("setalert is called for " + animal_detected)

    if animal_detected == "bear":
        print("It's a bear")
        urllib.request.urlopen(alert_url)
    elif animal_detected == "dog":
     # if dog = false, turn off the alert system
        urllib.request.urlopen(safe_url)

def run1():
    cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)

        cv2.imshow('live transmission',im)
        key=cv2.waitKey(5)
        if key==ord('q'):
            break
            
    cv2.destroyAllWindows()
        
def run2():
    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)

        # This part draws a box and a label with the detected object name 
        bbox, label, conf = cv.detect_common_objects(im)

        im = draw_bbox(im, bbox, label, conf)


        # Call the isWild and inject the label of the detecteed animal
        # isWild(label)
        print(label , " detected")
        animal_detected = label
        setAlert(animal_detected)

        cv2.imshow('detection',im)

        key=cv2.waitKey(5)
        if key==ord('q'):
            break
            
    cv2.destroyAllWindows()
 
 
 
if __name__ == '__main__':
    print("started")
    with concurrent.futures.ProcessPoolExecutor() as executer:
             f1= executer.submit(run1)
             f2= executer.submit(run2)



