import cv2
import matplotlib.pyplot as plt
import cvlib as cv
import urllib.request
import numpy as np
from cvlib.object_detection import draw_bbox
import concurrent.futures
 

# Change this URL based on the ip address granted to the ESP32 cam webserver 
url='http://192.168.0.130/cam-hi.jpg'


im=None
 
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

        # Call the isWild and inject the label of the detecteed animal
        isWild(label)
        im = draw_bbox(im, bbox, label, conf)
 
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


# Define wild animal and not wild animal 
def isWild(label):
    safeAnimalList = ["cat", "dog", "chicken"]
    wildAnimalList = ["beer", "lion"]

    if label in wildAnimalList:
        setAlert(true)
    else:
        setAlert(false)



# Based on the case call the alert System
def setAlert(isWild):
     if isWild:
     # if isWild = true , turn on the alert sytem
        urllib.request.urlopen('http://192.168.0.187/setWildStatus?s=1')
     else:
     # if isWild = false, turn off the alert system
        urllib.request.urlopen('http://192.168.0.187/setWildStatus?s=0')