# Foreground Removal Algorithm

This is an attempt to develope in C++ a foreground removal algorithm that I condded in Python. It is not ready yet.

Directorey Test/images has some images that can be used in a test and the fore_removed.png image was generated wit the Python script.

Next steps:

- [x] Finish to code the second part of `calcFittestBackgroudPixel`.
- [x] Add write capability to the Image class.
- [ ] Add concurrency to the `calcFittestBackgroudPixel` method. That should be easy as every pixel in the generated image caon b4e caulculated independently.
