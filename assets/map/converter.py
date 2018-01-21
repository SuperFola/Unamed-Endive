import sys;print(sys.argv[1])
with open(sys.argv[1],"r")as file:
    a=file.read().replace(" ", "").replace("\r\n", "").replace("\n\r", "").replace("\n", "").replace("\t", "").replace("\r", "").replace('True', 'true').replace('False', 'false').replace("},]", "}]").replace("},}", "}}").replace("],}", "]}").replace("}{", "},{").replace("]{", "],{").replace("}[", "},[").replace("][", "],[")
    if a[0] != "{": a = "{" + a
with open(sys.argv[1],"w")as file: file.write(a)