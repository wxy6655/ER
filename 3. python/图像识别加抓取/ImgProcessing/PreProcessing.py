import cv2
import copy
import numpy as np
from MainFunctions.VideoStream import VideoStream
from MainFunctions.Common import Common
from MainFunctions.Visualizer import Visualizer


class PreProcessing(object):


    def __init__(self):
        pass

    def preProcess(self,img):

        return img


    def binaryProcess(self, img, algorithm = 0, threshold = 127):
        '''
        :param img: image input
        :param algorithm: Algorithms: Threshold
            http://docs.opencv.org/3.1.0/d7/d4d/tutorial_py_thresholding.html
            algorithm 0: Binary image
            algorithm 1: Gaussian algorithm
            algorithm 2: Mean value algorithm
        :param threshold: usually should be 127 or lower
            Light - threshold up
            Dark - threshould down
        :return: image
        '''
        try :
            # try cvt image to gray unless the image is gray alright!
            gray_image = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
        except:
            gray_image = img
            pass

        img_output = copy.deepcopy(gray_image)
        if algorithm == 0:
            ret, img_output = cv2.threshold(gray_image, threshold, 255, cv2.THRESH_BINARY)
            #ret, img_output = cv2.threshold(gray_image, 0, 15, cv2.THRESH_BINARY)

        elif algorithm == 1:
            img_output = cv2.adaptiveThreshold(gray_image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        elif algorithm == 2:
            img_output = cv2.adaptiveThreshold(gray_image, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
        else:
            print "Error: Algorithm's number is incorrect!"
            return img

        return img_output

    def dilateAndErode(self, img, algorithm = 0, kernel_size = 2):
        '''
        :param img: image input
        :param algorithm: dilate and erode
            http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_morphological_ops/py_morphological_ops.html
            algorithm = 0: dialte - add more
            algorithm = 1: erode - delete small
        :param kernel_size: how many add or delete

        :return: image
        '''

        if kernel_size > 0:
            kernel = np.ones((kernel_size, kernel_size), np.uint8)
            if algorithm == 0:
                img_output = cv2.dilate(img, kernel, iterations=1)
                return img_output
            elif algorithm == 1:
                img_output = cv2.erode(img, kernel, iterations = 1)
                return img_output
            else:
                return img
        else:
            return img

    def noiseHandler(self, img, algorithm = 0, area_threshold = 30):
        '''

        :param img:
        :param algorithm:
                http://opencvpython.blogspot.jp/2012/06/hi-this-article-is-tutorial-which-try.html
        :param area_threhold:
                The size threshold which will be ignored
        :return:
        '''

        img_output = Common().getColoredCopy(img, color = (0,0,0))
        contours , _= cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        countor_new = []

        for h, cnt in enumerate(contours):
            area = cv2.contourArea(cnt)

            if area > area_threshold:
                countor_new.append(cnt)

        cv2.drawContours(img_output,countor_new,-1,(255,255,255),1)

        return img_output

    def preProcessingCombo(self, img, img_type):

        img = self.binaryProcess(img, algorithm=0, threshold=180)
        img = 255- img

        img = self.dilateAndErode(img, algorithm=1, kernel_size=5)
        #Visualizer().quickViewer(img)



        # delete small particle
        img = self.dilateAndErode(img, algorithm=0, kernel_size=3)
        #Visualizer().quickViewer(img)


        img = self.noiseHandler(img, algorithm=0, area_threshold=200)
        #Visualizer().quickViewer(img)

        if img_type == 'image':
            Visualizer().imgShow('testing_data',img)


        return img
