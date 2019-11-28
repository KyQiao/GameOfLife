import life as m
import cv2
import numpy as np
from time import time,sleep

SHOW_FPS = True
# SHOW_FPS = False
# CONTROL_FPS = True # 25 default 1/25=0.04
CONTROL_FPS = False

N1,N2 = 500,500
N = N1*N2//4


test = m.life(N1,N2,N,100)
ones = np.ones([N1*N2])
cv2.namedWindow('image',cv2.WINDOW_NORMAL)

if SHOW_FPS:
    fps=0

    while True:
        t0=time()
        test.update(1)
        img = (ones*test.data).reshape([N1,N2])
        cv2.imshow('image',img)
        t1 = time()
        if CONTROL_FPS:
            sleep(1/25-(t1-t0))
        t1 = time()
        fps = 1/(t1-t0)
        print(fps)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break

else:
    while True:
        test.update(1)
        img = (ones*test.data).reshape([N1,N2])
        cv2.imshow('image',img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break