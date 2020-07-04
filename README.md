This is a simple example using [VLfeat](vlfeat.org) and [CImg](http://cimg.eu/) to extract HOG features.

To build it, you need to install vlfeat, for Debian users:
```shell
apt install libvlfeat-dev
```
CImg is a header only library, you could simple download it to the source dir.

```shell
git clone https://github.com/hubutui/vlfeat-hog-example.git
cd vlfeat-hog-example
cmake -B build
make -C build
```

To generate HOG features, run:
```shell
./build/hog-example <input image> <feature.txt> <hog render image>
```
HOG features are extracted and saved to `feature.txt`, one value per line. So you could load in numpy with
```python
import numpy as np
hog = np.loadtxt("feature.txt", dtype=np.float32)
```