double applyFilter(class Filter *filter, cs1300bmp *local, cs1300bmp *output)
{

    long long cycStart, cycStop;

    cycStart = rdtscll();
    //create local var
    cs1300bmp * __restrict local = local;
    //Performing some calculations locally rather than in loop
    int w = (local -> width) - 1;
    int h = (local -> height) - 1;
//     int filterArr[]; //Localized filter array
//     filterArr[0] = filter -> get(0, 0);
//     filterArr[1] = filter -> get(0, 1);
//     filterArr[2] = filter -> get(0, 2);
//     filterArr[3] = filter -> get(1, 0);
//     filterArr[4] = filter -> get(1, 1);
//     filterArr[5] = filter -> get(1, 2);
//     filterArr[6] = filter -> get(2, 0);
//     filterArr[7] = filter -> get(2, 1);
//     filterArr[8] = filter -> get(2, 2);
    
    float div = (1.0/(filter -> getDivisor()));
    int plane;
    int sum;
    int x = output -> color[0][row][col];
    int y = output -> color[1][row][col];
    int z = output -> color[2][row][col];
    //Multithreading
    #pragma parallel for
    for(int row = 1; row < h; row++) 
    {
      for(int col = 1; col < w; col++) 
      {
        // int plane = 0;
        output -> color[0][row][col] = 0;
        output -> color[1][row][col] = 0;
        output -> color[2][row][col] = 0;

        for (int j = 0; j < 3; j++) 
        { //getSize will always be 3
            for (int i = 0; i < 3; i++) 
            { 
                output -> color[0][row][col] = output -> color[0][row][col] + (local -> color[0][row + i - 1][col + j - 1] * filter -> get(i, j));
                output -> color[1][row][col] = output -> color[1][row][col] + (local -> color[1][row + i - 1][col + j - 1] * filter -> get(i, j));
                output -> color[2][row][col] = output -> color[2][row][col] + (local -> color[2][row + i - 1][col + j - 1] * filter -> get(i, j));
            }
        } 
          if (div != 1)
        {
          output -> color[0][row][col] *= div;  

          output -> color[1][row][col] *= div;

          output -> color[2][row][col] *= div;
        }

        if ( output -> color[0][row][col]  < 0 ) 
        {
          output -> color[0][row][col] = 0;
        }  
        if ( output -> color[1][row][col]  < 0 ) 
        {
          output -> color[1][row][col] = 0;
        }  
        if ( output -> color[2][row][col]  < 0 ) 
        {
          output -> color[2][row][col] = 0;
        }
        if ( output -> color[0][row][col]  > 255 ) 
        { 
          output -> color[0][row][col] = 255;
        }
        if ( output -> color[1][row][col]  > 255 ) 
        { 
          output -> color[1][row][col] = 255;
        }
        if ( output -> color[2][row][col]  > 255 ) 
        { 
          output -> color[2][row][col] = 255;
        }
  
      }
    }
  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
        
/*
  // /////////////////// 2nd iteration
  // plane = 1;
  //   output -> color[plane][row][col] = 0;

  // for (int j = 0; j < filter -> getSize(); j++) { //getSize will always be 3
  //   for (int i = 0; i < filter -> getSize(); i++) { 
  //     output -> color[plane][row][col]
  //       = output -> color[plane][row][col]
  //       + (local -> color[plane][row + i - 1][col + j - 1] 
  //    * filter -> get(i, j) );
  //   }
  // }
  
  // output -> color[plane][row][col] =  
  //   output -> color[plane][row][col] / filter -> getDivisor();

  // if ( output -> color[plane][row][col]  < 0 ) {
  //   output -> color[plane][row][col] = 0;
  // }

  // if ( output -> color[plane][row][col]  > 255 ) { 
  //   output -> color[plane][row][col] = 255;
  // }

  // ///////////////// 3rd iteration
  // plane = 2;
  //   output -> color[plane][row][col] = 0;

  // for (int j = 0; j < filter -> getSize(); j++) { //getSize will always be 3
  //   for (int i = 0; i < filter -> getSize(); i++) { 
  //     output -> color[plane][row][col]
  //       = output -> color[plane][row][col]
  //       + (local -> color[plane][row + i - 1][col + j - 1] 
  //    * filter -> get(i, j) );
  //   }
  // }
  
  // output -> color[plane][row][col] =  
  //   output -> color[plane][row][col] / filter -> getDivisor();

  // if ( output -> color[plane][row][col]  < 0 ) {
  //   output -> color[plane][row][col] = 0;
  // }

  // if ( output -> color[plane][row][col]  > 255 ) { 
  //   output -> color[plane][row][col] = 255;
  // }
  /*
  int plane = 0;
  for(int plane = 0; plane < 3; plane++) {

  output -> color[plane][row][col] = 0;

  for (int j = 0; j < filter -> getSize(); j++) { //getSize will always be 3
    for (int i = 0; i < filter -> getSize(); i++) { 
      output -> color[plane][row][col]
        = output -> color[plane][row][col]
        + (local -> color[plane][row + i - 1][col + j - 1] 
     * filter -> get(i, j) );
    }
  }
  
  output -> color[plane][row][col] =  
    output -> color[plane][row][col] / filter -> getDivisor();

  if ( output -> color[plane][row][col]  < 0 ) {
    output -> color[plane][row][col] = 0;
  }

  if ( output -> color[plane][row][col]  > 255 ) { 
    output -> color[plane][row][col] = 255;
  }
      }
      
      
      
      
      
      
      /////////////////////////////////////////
    long long cycStart, cycStop;

    cycStart = rdtscll();
    //create local var
//     cs1300bmp * __restrict local = input;
//     //Performing some calculations locally rather than using class-defined functions
//     int w = output -> width = local -> width;
//     int h = output -> height = local -> height;
//     w = w - 1;
//     h = h - 1;
    
//     float div = (1.0/(filter -> getDivisor()));
//     //Multithreading
//     #pragma parallel for
//     for(int row = 1; row < h; row++) 
//     {
//       for(int col = 1; col < w; col++) 
//       {
//         // int plane = 0;
//         output -> color[0][row][col] = 0;
//         output -> color[1][row][col] = 0;
//         output -> color[2][row][col] = 0;

//         for (int j = 0; j < 3; j++) 
//         { //getSize will always be 3
//             for (int i = 0; i < 3; i++) 
//             { 
//                 output -> color[0][row][col] = output -> color[0][row][col] + (local -> color[0][row + i - 1][col + j - 1] * filter -> get(i, j));
//                 output -> color[1][row][col] = output -> color[1][row][col] + (local -> color[1][row + i - 1][col + j - 1] * filter -> get(i, j));
//                 output -> color[2][row][col] = output -> color[2][row][col] + (local -> color[2][row + i - 1][col + j - 1] * filter -> get(i, j));
//             }
//         } 
 
//         if (div != 1)
//         {
//           output -> color[0][row][col] *= div;  

//           output -> color[1][row][col] *= div;

//           output -> color[2][row][col] *= div;
//         }

//         if ( output -> color[0][row][col]  < 0 ) 
//         {
//           output -> color[0][row][col] = 0;
//         }  
//         if ( output -> color[1][row][col]  < 0 ) 
//         {
//           output -> color[1][row][col] = 0;
//         }  
//         if ( output -> color[2][row][col]  < 0 ) 
//         {
//           output -> color[2][row][col] = 0;
//         }
//         if ( output -> color[0][row][col]  > 255 ) 
//         { 
//           output -> color[0][row][col] = 255;
//         }
//         if ( output -> color[1][row][col]  > 255 ) 
//         { 
//           output -> color[1][row][col] = 255;
//         }
//         if ( output -> color[2][row][col]  > 255 ) 
//         { 
//           output -> color[2][row][col] = 255;
//         }
//     }
//   }
  //////////////////////////////////////