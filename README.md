# pose-annotation-tool

### Compiling the tool
```
cmake .
make
```

### Running the tool
./pose-annotation-tool path_to_list_images.txt


### Using the tool
The image is loaded and displayes on screen one by one.

Press ENTER to annoate current image

Press SPACE to skip current image


#### Once the image has been selected for annotation

Left Click - Visible joint

Right Click - Occluded joint

TAB - Current joint is not applicable for current image

Any key - register the annotation and move to next joint

#### After all the joints are annotated the joint coordinates is written to file and next image is loaded.
