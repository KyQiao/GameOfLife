# Game of Life
C++ implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), binding with Python


## Installation
require `Cmake`, c++ compiler(e.g. g++, MSVC) and some Python lib(`Opencv-python`,`NumPy`) to output animation.

Examples can be found in "tests" folder.

Install the package

```bash
python setup.py bdist_wheel
cd dist
pip install xxx.wheel
```

## Doc

There are two way of initializing the game:

```python
#all input is positive integer
m=life(height,width,init_number_of_particle,random_seed)

#you can use outside configuration too
m=life(height,width,np.random.randint(2,size=height*width))
#input configuration should be onedimension using c-style stored in memory.(use ravel)

#evolve with 4 process, 1 by default
m.update(4)

#get current state of game, resize(height,width) to illustrate.
data=m.getData()

plt.imshow(data.resize(height,width))
```

The update is very fast, 300 by 300 setup can reach ~200 fps on single core.
