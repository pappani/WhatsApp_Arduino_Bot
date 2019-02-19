##Pappani Federico - federicopappani.it
##WhatsApp Remote Manager

from selenium import webdriver
import sys, getopt
from flask import Flask, request, redirect, url_for, render_template
from selenium.webdriver.common.by import By
import time

app = Flask(__name__)

global status
status = 'Closed'

@app.route('/')
def nothing():
    global status
    output = 'Status: ' + status
    return (output)

@app.route('/quit')
def quit():
    global web, status
    try:
        web.close()
        status = 'Closed'
        return ("Closed")
    except:
        return ("Already closed")
        

@app.route('/begin/<passw>')
def begin():   #begin with the login
    global web, status
    status = 'Running'
    web = webdriver.Chrome()
    web.get('http://web.whatsapp.com')
    return("WhatsApp Manager\nReady")
    
@app.route('/view/<name>')
def view(name):     #view messages
    global web
    recentList = web.find_elements_by_class_name("_1wjpf")
    output = "Not found."
    for head in recentList:
        if head.text == name:
            head.click()
            output = "Found!"
            break
    time.sleep(3)
    elemt = web.find_elements_by_class_name("Tkt2p")
    for elemn in elemt:
        ##output += "<br/>" + elemn.text
        output += elemn.text
    elemf = {}
    elemf = web.find_elements_by_class_name("_3v3PK")
    if elemf != {}:
        output += "--- Photos ---\n"
    for elemn in elemf:
        output += "|Photo|\n"
    return output

@app.route('/check_new')
def check_new():      #check new messages
    global web
    num = ''
    title = web.title
    print(title)
    for char in title:
        if '0' <= char <= '9':
            num += char
    if num != '':
        output = num
        ##output = str('You have',num , 'new message(s)!')
        return output
    else:
        return 'No new messages.'

@app.route('/write/<name>/<message>')
def write(name, message):       #send message
    global web
    recentList = web.find_elements_by_class_name("_1wjpf")
    for head in recentList:
        if head.text == name:
            head.click()
            break
    output = "Found!"
    time.sleep(1)
    elem1 = web.find_element_by_css_selector('div._2S1VP.copyable-text.selectable-text')
    elem1.click()
    time.sleep(1)
    elem1.send_keys(message ,'\n')
    output += "\nSent!"
    return output

@app.route('/search/<name>')
def search(name):
    global web
    searchtab = web.find_element_by_css_selector("input.jN-F5.copyable-text.selectable-text")
    searchtab.click()
    time.sleep(1)
    searchtab.send_keys(name, '\n')
    time.sleep(3)
    elem = web.find_element_by_class_name("_1wjpf")
    if elem.text == name:
        elem.click()
        return "Found!"
    return "Found."

def main(argv):
    opt = getopt.getopt(argv, "")
    if opt != ([],[]):
        IP = str(opt[1]).strip("[]''")
        print("Working on ", IP)
        app.run(debug = True, host = IP, port = 80)
    else:
        print ("Run the code with IP address.")
        time.sleep(2)

if __name__ == "__main__":
    main(sys.argv[1:])

