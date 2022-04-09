from PreProcessing import PreProcessing
import cv2
import copy
import numpy as np
import math

import time

from MainFunctions.Visualizer import Visualizer


class TemplateMatching(object):
    def __init__(self, img, template, para = None):
        self.template   = template
        self.img        = img
        self.para       = para
        self.original_img = copy.deepcopy(img)

        if self.template is None or self.img is None:
            print "Error Template or Img is none in Template Matching Function"
            pass

        # Begin to process
        '''
        Step 1: Preprocessing
        Step 2: Template Matching and print results

        '''
        #self.img = self.preProcessing(self.img,img_type='image')
        #self.template = self.preProcessing(self.template, img_type='template')




        self.templateMatchingMethod(self.img, self.template)




        self.last_angle = 0

        #print "para here is " + str(para)


        pass



    def preProcessing(self, img, para = {}, img_type= 'image'):
        '''
        Bug 1 : delete first line will cause crash because of the image problem
        '''
        para = 0
        processer = PreProcessing()
        img = processer.preProcessingCombo(img, img_type)
        return img


    def templateMatchingMethod(self, img, template, algorithm = 0, threshold = 0.8):   #0.23 for sb object

        '''
        :param img:
        :param template:
        :param algorithm:
            http://docs.opencv.org/3.1.0/d4/dc6/tutorial_py_template_matching.html

            transform matrix
            http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_geometric_transformations/py_geometric_transformations.html
        :param threshold:
            0.4 ~ 0.5 for contour imgs
            low threshold which will lead multiple recognition and high -> no img

            use the robot arm to find the best threshold


        :return:
        '''

        '''

        Bug 2: There may be a size problem -> template too bigger than image -> cause failed matchTemplate

        '''

        rotat_range = 60
        h, w, _ = template.shape

        _offset = 2
        template = template[_offset :h- 2*_offset, _offset:w-2*_offset]


        h_o = 74.0
        w_o = -20.0
        l = math.sqrt(h_o * h_o + w_o * w_o)
        theta_0 = math.atan(w_o/h_o)

        i =  -rotat_range/2

        trans = math.pi/180
        trigger = True


        while i < rotat_range/2 and trigger == True:
        #if trigger == True:
            #i = -30

            #print i
            M = cv2.getRotationMatrix2D((w / 2, h / 2), i, 1)
            M_out = cv2.warpAffine(template, M, (w, h))

            '''
            --------------------------------------------------------
            '''

            '''
            input is Mout and img
            '''

            ratio = 0.5
            height, width = img.shape[:2]
            img_resize = cv2.resize(img, (int(ratio * width), int(ratio * height)), interpolation=cv2.INTER_CUBIC)



            height, width = M_out.shape[:2]
            M_out_resize = cv2.resize(M_out, (int(ratio * width), int(ratio * height)), interpolation=cv2.INTER_CUBIC)

            w_of_template = int(ratio * width)
            h_of_template = int(ratio * height)

            time1 = time.time()
            result = cv2.matchTemplate(M_out_resize, img_resize, cv2.TM_CCOEFF_NORMED)
            loc = np.where(result >= threshold)
            time2 = time.time()

            a               = None
            b               = None
            _rotation_angle = None

            #print 'time is ', time2 - time1

            '''
            Here can verify the difference between imgcrop(final img) and M_out_resize

            '''

            def checkArea(point, threhold_percentage = 0.3):


                img_crop = img_resize[point[1]:(point[1] + h_of_template), point[0]:(point[0] +w_of_template)]

                def findArea(_img_input,type_of_image):

                    _h,_w, _ = _img_input.shape
                    blank_image = cv2.cvtColor(_img_input,cv2.COLOR_RGB2GRAY)
                    contours, _ = cv2.findContours(blank_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                    area_total = 0

                    for h, cnt in enumerate(contours):
                        area = cv2.contourArea(cnt)
                        area_total += area

                    return area_total

                img_crop_area = findArea(img_crop, type_of_image = 'imgcrop')
                template_area = findArea(M_out_resize, type_of_image ='M_out_resize' )

                if img_crop_area == 0:
                    return False

                percentage = abs(template_area-img_crop_area)/img_crop_area

                if percentage < threhold_percentage:
                    return True
                else:
                    return False


            for pt in zip(*loc[::-1]):

                a = int(pt[0] / ratio)
                b = int(pt[1] / ratio)

                if checkArea(pt):

                    cv2.rectangle(self.original_img, (a, b), (a + w, b + h), (0, 0, 255), 2)

                    if len(pt) > 1:
                        # print 'Rotation angle is ' + str(i)

                        _rotation_angle = copy.deepcopy(i)

                        theta = theta_0 + i * trans

                        trigger = False

                        break

            else:
                pass



            i+= 1




            #self.output = copy.deepcopy(template)
        self.output = copy.deepcopy(self.original_img)
        self.obj_coord = (a, b,_rotation_angle)



    def test(self):

        print "shabi"

    def getResult(self):
        return self.output , self.obj_coord


