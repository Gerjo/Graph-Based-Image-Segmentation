/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <cstdio>
#include <cstdlib>
#include <image.h>
#include <misc.h>
#include <pnmfile.h>
#include <vector>
#include "segment-image.h"

#include "jpeg-compressor/jpgd.h"
#include "jpeg-compressor/jpge.h"

#include <iostream>

#include "lodepng.h"

int main(int argc, char **argv) {
	
  if (argc != 6) {
    fprintf(stderr, "usage: %s sigma k min input(jpg) output(png)\n", argv[0]);
    fprintf(stderr, "  Typical parameters are sigma = 0.5, k = 500, min = 20.");
    fprintf(stderr, "  Larger values for k result in larger components in the result.");

    return 1;
  }
  
  float sigma = atof(argv[1]);
  float k = atof(argv[2]);
  int min_size = atoi(argv[3]);


  int w = 0;
  int h = 0;
  int channels = 0;
  
  printf("sigma: %f\n", sigma);
  printf("k: %.0f\n", k);
  printf("min_size: %d\n", min_size);
  
  printf("Loading input jpeg image\n");
  unsigned char* data = jpgd::decompress_jpeg_image_from_file(argv[4], &w, &h, &channels, 3);

  printf("width: %d, height: %d, channels: %d\n", w, h, channels);
	
  image<rgb> im(w, h, false);

  unsigned char* dst = (unsigned char*) im.data;

  printf("Copying data into image<rgb> structure\n");
  for(int i = 0; i < w * h * channels; ++i) {
	dst[i] = data[i];
  }
	
  //printf("loading input image.\n");
  //image<rgb> *input = loadPPM(argv[4]);
	
  printf("processing\n");
  int num_ccs; 
  image<rgb> *seg = segment_image(&im, sigma, k, min_size, &num_ccs); 
  //savePPM(seg, argv[5]);

  
  std::vector<unsigned char> imvec_rgba(w * h * 4, 0);
  
  // Copy into std::vector
  for(int read = 0, write = 0; write < imvec_rgba.size(); read += 1, write += 4) {
	  imvec_rgba[write + 0] = seg->data[read].r;
	  imvec_rgba[write + 1] = seg->data[read].g;
	  imvec_rgba[write + 2] = seg->data[read].b;
	  imvec_rgba[write + 3] = 255;
  }

  //Encode the image
  unsigned error = lodepng::encode(argv[5], imvec_rgba, w, h);

  //if there's an error, display it
  if(error) {
	  std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
  }
  
  //jpge::compress_image_to_jpeg_file(argv[5], w, h, channels, (unsigned char*) seg->data);
	

  printf("got %d components\n", num_ccs);
  printf("done! uff...that's hard work.\n");

  return 0;
}

