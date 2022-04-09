
from PreProcessing import PreProcessing
import cv2

import copy
from MainFunctions.Common import  Common
import numpy as np
from matplotlib import pyplot as plt
from MainFunctions.Visualizer import Visualizer


class SiftDetection(object):

    def __init__(self, img, template, para = None):
        self.template = template
        self.img = img
        self.para = para
        self.imgOriginal = copy.deepcopy(img)

        #self.img = PreProcessing().preProcessingCombo(self.img)

        #PreProcessing().preProcessingCombo(self.template)

        '''
        Main algorithms

        '''



        kp_img, des_img = self.findKeyPoints(self.img)
        kp_tem, des_tem = self.findKeyPoints(self.template)

        drawKp = self.drawKeyPoints(self.img, kp_img)

        result = self.drawMatches(self.template, self.img, kp_tem, des_tem, kp_img, des_img, algorithm = 'flann')

        self.result = copy.deepcopy(self.img)
        pass

    def findKeyPoints(self,img):

        sift = cv2.SIFT()
        keypoints, descriptor = sift.detectAndCompute(img, None)

        return (keypoints,descriptor)

    def drawKeyPoints(self, img, keypoints):

        img_copy = copy.deepcopy(img)
        cv2.drawKeypoints(img, keypoints, img_copy, (255, 0, 0))


        return img_copy

    def drawMatches(self, template, img, kp1, des1, kp2, des2, algorithm = 'flann'):

        '''

        :param img:
        :param template:
        :param kp1:
        :param des1:
        :param kp2:
        :param des2:
        :param algorithm:
            http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_feature2d/py_matcher/py_matcher.html
        :return:

        '''

        if algorithm == 'bf':
            bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

            # Match descriptors.
            matches = bf.match(des1, des2)

            # Sort them in the order of their distance.
            matches = sorted(matches, key=lambda x: x.distance)

            # Draw first 10 matches.



            #print len(good)

            if len(matches)>3:

                blank_image = np.zeros((640, 480, 3), np.uint8)
                imgOutput = cv2.drawMatches(img, kp1, template, kp2, matches, self.imgOriginal, matchColor = (255,255,255))

                src_pts = np.float32([kp1[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp2[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)

                M, mask = cv2.findHomography( dst_pts,src_pts, cv2.RANSAC, 1.0)

                h, w, _ = template.shape
                pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)

                dst = cv2.perspectiveTransform(pts, M)
                #print M



                #cv2.drawKeypoints(frame_input, kp2, blank_image, color=(0, 255, 0), flags=0)
                #blank_image2 = cv2.drawMatches(img1, kp1, img2, kp2, matches)




                cv2.polylines(self.imgOriginal, [np.int32(dst)], True, (255, 255, 255), 2, cv2.LINE_AA)

            else:


                print "not enough matches"

            '''
            good = []
            for m in matches:
                if m.distance < 1 :
                    good.append(m)

            print good
            '''

        elif algorithm == 'flann':

            FLANN_INDEX_KDTREE = 0
            index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
            search_params = dict(checks = 50)  # or pass empty dictionary

            flann = cv2.FlannBasedMatcher(index_params, search_params)

            matches = flann.knnMatch(des1, des2, k=2)

            imgOutput = np.zeros((480, 640, 3), np.uint8)
            # print "matches are " + str(len(matches))

            #print len(matches)

            MIN_MATCH_COUNT = 3

            if len(matches) > MIN_MATCH_COUNT:
                src_pts = np.float32([kp1[m[0].queryIdx].pt for m in matches]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp2[m[0].trainIdx].pt for m in matches]).reshape(-1, 1, 2)

                M, mask = cv2.findHomography( src_pts, dst_pts, cv2.RANSAC, 3.0)

                h, w, channels = template.shape
                pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
                dst = cv2.perspectiveTransform(pts, M)

                #cv2.drawKeypoints(frame_input, kp2, blank_image, color=(0, 255, 0), flags=0)

                #blank_image2 = drawMatches(img1, kp1, img2, kp2, matches)

                points =  np.int32(dst)

                area_size = cv2.contourArea(points)

                if area_size>5000:
                    cv2.polylines(self.imgOriginal, np.int32([dst]), True, (255, 255, 255), 2, cv2.CV_AA)


            else:
                print "Not enough matches are found - %d/%d" % (len(matches), MIN_MATCH_COUNT)


        else:
            print "Error: Input Algorithm is wrong in Matching Keypoints"

        #plt.imshow(img3),plt.show()

        return imgOutput




    def getResult(self):
        return self.imgOriginal
        #return self.result
