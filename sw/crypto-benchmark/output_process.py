import os,sys
input_flie = open("output.txt","r",encoding="utf-8")
output_file = open("check.py","w",encoding="utf-8")

wflag = False
newline = []

for line in input_flie :
    if "Info" in line :
        wflag = False
    if "import" in line :
        wflag = True
    if wflag == True :
        K = list(line)
        if len(K)>1 :
            for i in K:
                newline.append(i)

strlist = "".join(newline)
newlines = str(strlist)

output_file.write(newlines)

output_file.close()
input_flie.close()
    
    