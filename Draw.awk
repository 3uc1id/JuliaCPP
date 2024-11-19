#!/bin/awk -f

function parse_inputs(l, n){
    if(!(l[n])){
        return
    } else if((l[n] == "-H") || (l[n] == "--help")){
        success = 0
        system("cat help")
        return
    } else if((l[n] == "-B") || (l[n] == "--bottomright")){
        split(l[n+1], br, ",")
        bottom_right1 = br[1]
        bottom_right2 = br[2]
    } else if((l[n] == "-T") || (l[n] == "--topleft")){
        split(l[n+1], br, ",")
        top_left1 = br[1]
        top_left2 = br[2]
    } else if((l[n] == "-C") || (l[n] == "--cparam")){
        split(l[n+1], br, ",")
        c1 = br[1]
        c2 = br[2]
    } else if((l[n] == "-O") || (l[n] == "--filename")){
        filename = l[n+1]
    } else if((l[n] == "-H") || (l[n] == "--height")){
        height = l[n+1]
    } else if((l[n] == "-I") || (l[n] == "--iterations")){
        iterations = l[n+1]
    } else if((l[n] == "-R") || (l[n] == "--radius")){
        radius = l[n+1]
    } else if((l[n] == "-r") || (l[n] == "--randompalette")){
        random_palette = l[n+1]
    } else if((l[n] == "-N") || (l[n] == "--threads")){
        threads = l[n+1]
    } else if((l[n] == "-W") || (l[n] == "--width")){
        width = l[n+1]
    } else {
        success = 0
        print("ERROR: unknown argument " l[n])
    }
    return(parse_inputs(l, n+2))
}

BEGIN {

    success = 1
    
    #default values
      iterations = 2500
      threads = 8
      height = 900
      width = 1600
      c1 = 0.35
      c2 = 0.35
      top_left1 = -2.0
      top_left2 = 1.125
      bottom_right1 = 2.0
      bottom_right2 = -1.125
      filename = "Julia.ppm"
      radius = 100.0
      random_palette = 0

      #load values from input
      split(ARGV[1], input, " ")
      parse_inputs(input, 1)

      #warnings for if values are outside of reasonable ranges.
      if(iterations < 5){
          print("WARNING: iterations is too low")
      }
      if(iterations > 50000){
          print("WARNING: iterations very high, the program will take a long time to run")
      }
      if(threads < 1){
          threads = 1
      }
      if(threads > 128){
          print("WARNING: using the maximum of 128 threads")
          threads = 128
      }
      if(height < 1){
          height = 1
      }
      if(height > 20000){
          print("WARNING: height too large. the program will take a long time to run")
      }
      if(width < 1){
          width = 1
      }
      if(width > 20000){
          print("WARNING: width too large. the program will take a long time to run")
      }
      if(radius < 0.1){
          print("WARNING: radius too small. this will draw something uninteresting")
      }
      if(c1 < -10){
          print("WARNING: real part of C is too small. will draw something uninteresting")
      }
      if(c1 > 10){
          print("WARNING: real part of C is too big. will draw something uninteresting")
      }
      if(c2 < -10){
          print("WARNING: imaginary part of C is too small. will draw something uninteresting")
      }
      if(c2 > 10){
          print("WARNING: imaginary part of C is too big. will draw something uninteresting")
      }

      #call c++ program
      if(success == 1){
          system("./JuliaCPP " iterations " " threads " " height " " width " " radius " " c1 " " c2 " " top_left1 " " top_left2 " " bottom_right1 " " bottom_right2 " " filename " " random_palette)
      }
}
