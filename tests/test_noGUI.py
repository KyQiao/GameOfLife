import life as m
import time
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

start = time.time()
interval = 0

while interval<30:
    # using 2 process, 1 by default
    test.update(2)
    img = (ones * test.getData()).reshape([N1, N2])
    frames += 1
    end = time.time()
    interval=end-start
print("average fps: ", frames / (end - start))
