import cv2


from AlgorithmTemplateMatching import TemplateMatching
from AlgorithmORBDetection import OrbDetection
from AlgorithmSIFTDetection import SiftDetection



class ImgProcessing(object):
    def __init__(self):
        pass

    def templateMatching(self, img, template, para):
        tm = TemplateMatching(img, template, para)
        return tm.getResult()
        pass

    def siftDetection(self, img, template, para):
        sift = SiftDetection(img, template, para)
        return sift.getResult()
        pass



    def orbDetection(self, img, template, para):
        od = OrbDetection(img, template, para)
        return od.getResult()
        pass
