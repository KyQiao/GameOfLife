import life as m
import time
# python-opencv is used
import cv2
import numpy as np


N1, N2 = 300, 300
N = N1 * N2 // 4

# initialization

# using random generator in c++
# test = m.life(N1, N2, N, 1343145)

# input outside configuration
test = m.life(N1, N2, np.random.randint(2, size=N1 * N2))

# calculate average fps
frames = 0

ones = np.ones([N1 * N2])
cv2.namedWindow('image', cv2.WINDOW_NORMAL)

start = time.time()

while True:
    # using 2 process, 1 by default
    test.update(2)
    img = (ones * test.getData()).reshape([N1, N2])
    cv2.imshow('image', img)
    frames += 1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        cv2.destroyAllWindows()
        break
end = time.time()
print("average fps: ", frames / (end - start))
