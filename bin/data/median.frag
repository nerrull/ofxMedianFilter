#version 150
uniform sampler2DRect tex0;
uniform vec2 u_resolution;
uniform int index;
in vec2 texCoordVarying;
out vec4 outputColor;

vec3 rgb2hsb( in vec3 c ){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

void SWAP(in int a, in int b, inout int[9] sort,inout  vec3[9] tmp) {
  if (sort[a]>sort[b]) {
    int t = sort[a];
    sort[a]=sort[b];
    sort[b]=t;

    vec3 t2 = tmp[a];
    tmp[a]=tmp[b];
    tmp[b]=t2;
  }
}

 void sort(inout vec3[9] pixels,inout int sortValues[9]) {
  SWAP(0, 1, sortValues, pixels);
  SWAP(3, 4, sortValues, pixels);
  SWAP(6, 7, sortValues, pixels);
  SWAP(1, 2, sortValues, pixels);
  SWAP(4, 5, sortValues, pixels);
  SWAP(7, 8, sortValues, pixels);
  SWAP(0, 1, sortValues, pixels);
  SWAP(3, 4, sortValues, pixels);
  SWAP(6, 7, sortValues, pixels);
  SWAP(0, 3, sortValues, pixels);
  SWAP(3, 6, sortValues, pixels);
  SWAP(0, 3, sortValues, pixels);
  SWAP(1, 4, sortValues, pixels);
  SWAP(4, 7, sortValues, pixels);
  SWAP(1, 4, sortValues, pixels);
  SWAP(2, 5, sortValues, pixels);
  SWAP(5, 8, sortValues, pixels);
  SWAP(2, 5, sortValues, pixels);
  SWAP(1, 3, sortValues, pixels);
  SWAP(5, 7, sortValues, pixels);
  SWAP(2, 6, sortValues, pixels);
  SWAP(4, 6, sortValues, pixels);
  SWAP(2, 4, sortValues, pixels);
  SWAP(2, 3, sortValues, pixels);
  SWAP(5, 6, sortValues, pixels);
//  return pixels;
}

bool checkBounds(vec2 pos){
    if ( pos.x ==u_resolution.x || pos.x ==0){
        return false;
    }
    if ( pos.y ==u_resolution.y || pos.y ==0){
        return false;
    }
    return true;
}

void getPixelValues( out vec3[9] pixels, out int[9] sortvalues){
    vec2 pixelSize = 1./u_resolution;

    //vec2((x-1)*pixelSize.x + (y-1)*pixelSize.y)
    for (int y = 0; y<=2; y++){
        for (int x = 0; x<=2; x++){
            vec3 rgb = texture(tex0, texCoordVarying + vec2(x-1,y-1)).rgb;
            vec3 hsb = rgb2hsb(rgb);
            pixels[y*3 +x] =rgb;
            sortvalues[y*3 +x]=  int(hsb.r*360)<<24&0xff000000 | int(rgb.r*255)<<16|int(rgb.g*255)<<8 |int(rgb.b*255);

        }
    }
}

void main(void)
{
    vec2 pos =gl_FragCoord.xy;
    vec3 pixelValue = texture(tex0, texCoordVarying).rgb;

    if (! checkBounds(pos)) {
        outputColor = vec4(pixelValue.rgb, 1.);
        return;
    }

    vec3 pixels[9];
    int sortValues[9];

    getPixelValues(pixels, sortValues);
    sort(pixels, sortValues);
    outputColor = vec4(pixels[index].r,pixels[index].g, pixels[index].b, 1.);
    //outputColor = vec4(0.,1.,0.,1.);

}
