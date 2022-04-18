# Requirements: matplotlob (`pip install matplotlib`)
# Usage: In the `fish` directory:
#   `python3 helpers/plot_testcases tests/plots tests`
#   Results are in `tests/plots`

import os
import shutil
import sys

import matplotlib.pyplot as plt


def generate_plot_from_file(tc_file, tc_dir, img_dir):
  plt.close()

  print('Generating {}'.format(tc_file))
  with open(os.path.join(tc_dir, tc_file) + '.in') as fp:
    fp.readline()
    N, M = map(int, fp.readline().strip().split())
    fish = [list(map(int, fp.readline().strip().split())) for _ in range(M)]

    X, Y, W = zip(*fish)
    fig, ax = plt.subplots()
    ax.scatter(X, Y, c=W, cmap='gray', marker='.')
    ax.set(xlim=(0, N), ylim=(0, N))

    plt.savefig(os.path.join(img_dir, tc_file) + '.png')


def main():
  if len(sys.argv) < 3:
    print("Usage: {} <image_dir> <tc_dir>".format(sys.argv[0]))
    print("       e.g. {} tests/plots tests".format(sys.argv[0]))
    return
  
  image_dir, tc_dir = sys.argv[1], sys.argv[2]
  print('Generating plot to folder {}...'.format(image_dir))

  if os.path.exists(image_dir):
    shutil.rmtree(image_dir)
  os.makedirs(image_dir)

  for filename in os.listdir(tc_dir):
    tc_file, ext = os.path.splitext(filename)
    if ext != '.in':
      continue
    generate_plot_from_file(tc_file, tc_dir, image_dir)


if __name__ == '__main__':
  main()
