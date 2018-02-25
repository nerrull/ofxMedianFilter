#version 150


uniform sampler2D tex0;
uniform vec2 u_resolution;
uniform int index;
uniform int mode;

in vec4 texCoordVarying;
out vec4 outputColor;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

int getmode(in vec4 col) {
  vec3 hsv = rgb2hsv(vec3(col));
  return int(hsv[mode]*255.0);
}

void SWAP(inout int[9] tmp, in int a, in int b) {
  int va = tmp[a];
  int vb = tmp[b];
  tmp[a] = min(va,vb);
  tmp[b] = max(va,vb);
}

void sort(inout int[9] tmp) {
  SWAP(tmp,0, 1);
    SWAP(tmp,3, 4);
    SWAP(tmp,6, 7);
    SWAP(tmp,1, 2);
    SWAP(tmp,4, 5);
    SWAP(tmp,7, 8);
    SWAP(tmp,0, 1);
    SWAP(tmp,3, 4);
    SWAP(tmp,6, 7);
    SWAP(tmp,0, 3);
    SWAP(tmp,3, 6);
    SWAP(tmp,0, 3);
    SWAP(tmp,1, 4);
    SWAP(tmp,4, 7);
    SWAP(tmp,1, 4);
    SWAP(tmp,2, 5);
    SWAP(tmp,5, 8);
    SWAP(tmp,2, 5);
    SWAP(tmp,1, 3);
    SWAP(tmp,5, 7);
    SWAP(tmp,2, 6);
    SWAP(tmp,4, 6);
    SWAP(tmp,2, 4);
    SWAP(tmp,2, 3);
    SWAP(tmp,5, 6);
}

int toint(in vec4 col) {
  ivec4 b = ivec4(col * 255.0);
  int res = (getmode(col) << 24) | (b.r << 16) | (b.g << 8) | (b.b);
  return res;
}

vec4 fromint(in int col) {
  vec4 res;

  res.r = ((col >> 16) & 0xff) / 255.0;
  res.g = ((col >> 8) & 0xff) / 255.0;
  res.b = (col & 0xff) / 255.0;
  res[3] = 1.0;

  return res;
}

void main(void) {


  vec2 texOffset =1./u_resolution;


  vec2 tc0 = texCoordVarying.st + vec2(-texOffset.s, -texOffset.t);
  vec2 tc1 = texCoordVarying.st + vec2(         0.0, -texOffset.t);
  vec2 tc2 = texCoordVarying.st + vec2(+texOffset.s, -texOffset.t);

  vec2 tc3 = texCoordVarying.st + vec2(-texOffset.s,          0.0);
  vec2 tc4 = texCoordVarying.st + vec2(         0.0,          0.0);
  vec2 tc5 = texCoordVarying.st + vec2(+texOffset.s,          0.0);

  vec2 tc6 = texCoordVarying.st + vec2(-texOffset.s, +texOffset.t);
  vec2 tc7 = texCoordVarying.st + vec2(         0.0, +texOffset.t);
  vec2 tc8 = texCoordVarying.st + vec2(+texOffset.s, +texOffset.t);

  int col0 = toint(texture2D(tex0, tc0));
  int col1 = toint(texture2D(tex0, tc1));
  int col2 = toint(texture2D(tex0, tc2));
  int col3 = toint(texture2D(tex0, tc3));
  int col4 = toint(texture2D(tex0, tc4));
  int col5 = toint(texture2D(tex0, tc5));
  int col6 = toint(texture2D(tex0, tc6));
  int col7 = toint(texture2D(tex0, tc7));
  int col8 = toint(texture2D(tex0, tc8));

  int[9] tmp = int[9](col0, col1, col2, col3, col4, col5, col6, col7, col8);
  //sort(tmp);

  outputColor = fromint(tmp[index]);

}
