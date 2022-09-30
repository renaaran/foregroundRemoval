# -*- coding: utf-8 -*-
"""
Created on Mon Mar 30 14:35:36 2020

Foregrounf removal simple algorithm

@author: Renato B. Arantes
"""

import os
import time
import argparse
import numpy as np
import matplotlib.pyplot as plt

from tqdm import tqdm
from PIL import Image
from typing import List
from scipy import ndimage

parser = argparse.ArgumentParser()
parser.add_argument('--imgroot', required=True, help='source images path')
opt = parser.parse_args()

def calc_fittest_backg_pixel(X:np.array, thresh:float = 0.5,
    min_inliers:int = 50, num_steps:int = 5):

    n = 0
    num_inliers = 0
    best_inliers = 0

    while num_inliers < min_inliers and n < 10:
        c = np.random.randint(0, len(X))
        x_mean = X[c]
        inliers = np.linalg.norm(X-x_mean, axis=1) < thresh
        num_inliers = sum(inliers)
        if num_inliers > best_inliers:
            bigger_inliers = inliers
            best_inliers = num_inliers
        n += 1

    for _ in range(num_steps):
        x_inliers = X[bigger_inliers]
        x_mean = np.mean(x_inliers, axis=0)
        bigger_inliers = np.linalg.norm(X-x_mean, axis=1) < thresh

    return np.mean(x_inliers, axis=0)

def read_dir(path:str, ftypes:List[str]) -> np.array:
    ft = set(['.'+t for t in ftypes])
    for fn in os.listdir(path):
        _, e = os.path.splitext(fn)
        if e in ft:
            yield fn

def load_images(path:str, ftypes:List[str]) -> np.array:
    imgs = []
    for fn in read_dir(path, ftypes):
        img = np.array(Image.open(os.path.join(path, fn)))
        imgs.append(img)
        if len(imgs) > 0:
            assert imgs[-1].shape == img.shape

    return np.array(imgs)

def get_background():
    imgs = load_images(opt.imgroot, ['jpg', 'png'])

    H, W, C = imgs[0].shape
    backg = np.zeros(shape=(H, W, C), dtype=np.uint8)

    for y in tqdm(range(H)):
        for x in range(W):
            backg[y, x] = calc_fittest_backg_pixel(imgs[:, y, x])

    return backg

if __name__ == '__main__':
    t = time.time()
    backg = get_background()
    Image.fromarray(backg).save('./backg_removed.png')
    print('execution time: %.2f' % (time.time()-t))
    plt.imshow(backg)
    plt.show()