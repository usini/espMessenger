from css_html_js_minify import html_minify, css_minify
from jsmin import jsmin
import os

def convertToGZIP(filename, varname, new_filename):
  with open(filename, 'r') as index_file:
    index_raw = index_file.read()

  index_minified = html_minify(index_raw)
  index_minified = css_minify(index_minified)
  index_minified = jsmin(index_minified)
  #print(index_minified)

  with open(filename + ".min",'w') as index_min_file:
    index_min_file.write(index_minified)

  try:
    os.unlink(filename + ".min.gz")
  except:
    pass

  try:
    os.system('gzip ' + filename + ".min")
  except:
    pass

  try:
    os.unlink(filename + ".min")
  except:
    pass
  hex_str = "const char " + varname + "[] PROGMEM={"
  with open(filename + ".min.gz", 'rb') as f:
      block = f.read()
      for ch in block:
            hex_str += hex(ch) + ", "

  try:
    os.unlink(filename + ".min.gz")
  except:
    pass

  index_final = hex_str + "};"
  with open(new_filename,'w') as index_final_file:
    index_final_file.write(index_final)

convertToGZIP("index.html", "HTTP_INDEX", "../web_index.h")
convertToGZIP("settings.html", "HTTP_SETTINGS", "../web_settings.h")
