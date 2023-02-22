import numpy as np
import matplotlib.pyplot as plt
import cv2
from skimage import data, color
from skimage.transform import hough_circle, hough_circle_peaks
from skimage.feature import canny
from skimage.draw import circle_perimeter
from skimage.util import img_as_ubyte

# Load picture and detect edges
image = cv2.imread("image/1.jpg", cv2.IMREAD_GRAYSCALE)

ret,th1 = cv2.threshold(image,35,255,cv2.THRESH_BINARY)
edgesint= canny(th1, sigma=3, low_threshold=150, high_threshold=255)

ret2,th2 = cv2.threshold(image,100,255,cv2.THRESH_BINARY)
edgesext = canny(th2, sigma=3, low_threshold=150, high_threshold=255)

# calculate hough radius
hough_radiii = np.arange(10, 2000)
hough_resi = hough_circle(edgesint, hough_radiii)

hough_radiie = np.arange(50, 2000)
hough_rese = hough_circle(edgesext, hough_radiie)

# Select the most prominent 2 circles
accumsi, cxi, cyi, radiii = hough_circle_peaks(hough_resi, hough_radiii,
                                           total_num_peaks=1)

accumse, cxe, cye, radiie = hough_circle_peaks(hough_rese, hough_radiie,
                                           total_num_peaks=1)

# drawing cercles
imagetest = color.gray2rgb(image)
for center_yi, center_xi, radiusi, center_ye, center_xe, radiuse, in zip(cyi, cxi, radiii,cye, cxe, radiie):
    circyi, circxi = circle_perimeter(center_yi, center_xi, radiusi,
                                    shape=image.shape)
    imagetest[circyi, circxi] = (220, 20, 20)
    circye, circxe = circle_perimeter(center_ye, center_xe, radiuse,
                                    shape=image.shape)
    imagetest[circye, circxe] = (225, 0, 0)
  

nl= imagetest.shape[0]
nc=  imagetest.shape[1]

Fcint= radiusi
Fcext= radiuse
xe = np.arange(-cxe,nc-cxe,1)
ye = np.arange(-cye,nl-cye,1)
xxe, yye = np.meshgrid(xe, ye, sparse=True)
ze = np.sqrt(xxe*xxe + yye*yye)
xi = np.arange(-cxi,nc-cxi,1)
yi = np.arange(-cyi,nl-cyi,1)
xxi, yyi = np.meshgrid(xi, yi, sparse=True)
zi = np.sqrt(xxi*xxi + yyi*yyi)

H5 = (ze<Fcext)*(Fcint<zi)*(image) 
 
plt.figure().set_size_inches(25,25)
plt.imshow(H5, cmap=plt.cm.gray)
plt.savefig('IrisSegmentation.png')
plt.imshow()
