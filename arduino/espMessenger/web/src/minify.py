from css_html_js_minify import html_minify, css_minify
from jsmin import jsmin
import os

def convertToGZIP(filename, varname, new_filename):

  with open(filename, 'r') as index_file:
    index_raw = index_file.read()

  # Minize file
  ext = os.path.splitext(filename)[1]

  if ext == ".css":
    print(filename + " ... " + " CSS")
    index_minified = css_minify(index_raw)
  elif ext == ".js":
    print(filename + " ... " + " JS")
    index_minified = jsmin(index_raw)
  else:
    print(filename + " ... " + " HTML")
    index_minified = html_minify(index_raw)
    index_minified = css_minify(index_minified)
    index_minified = jsmin(index_minified)
    index_minified = index_minified.replace('@charset"utf-8";','')

  #print("--")
  #print(index_minified)
  # Save Minize file to tempory file
  with open(filename + ".min",'w') as index_min_file:
    index_min_file.write(index_minified)

  # Remove gz file if exists
  try:
    os.unlink(filename + ".min.gz")
  except:
    pass

  # Compress file (need gzip)
  try:
    os.system('gzip ' + filename + ".min")
  except:
    pass

  # Remove minimize file 
  try:
    os.unlink(filename + ".min")
  except:
    pass

  # Generate hex_str
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

convertToGZIP("index.html", "MES_INDEX", "../web_index.h")
convertToGZIP("index_fr.html", "MES_INDEX", "../web_index_fr.h")
convertToGZIP("script.js", "MES_SCRIPT", "../web_script.h")
convertToGZIP("script_fr.js", "MES_SCRIPT", "../web_script_fr.h")
convertToGZIP("toast.min.js", "MES_TOAST", "../web_toast.h")
convertToGZIP("style.css", "MES_STYLE", "../web_style.h")
print("... Done!")