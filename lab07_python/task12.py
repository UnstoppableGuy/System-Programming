from tkinter import *
import threading
import time

root = Tk()

root.geometry("1000x500")
root.resizable(0, 0)

canvas = Canvas(root, width=1000, height=500)
canvas.pack()
square_1 = canvas.create_polygon((0, 0), (1000, 0), (1000, 500), (0, 500), fill="white")

def main():

    start_threads()

    root.mainloop()
    
def start_threads():
    threading.Thread(target=change_color, args=(1,)).start()
    threading.Thread(target=change_color, args=(2,)).start()
    threading.Thread(target=change_color, args=(3,)).start()


def change_color(index):
    colors = ["red", "yellow", "green"]
    lock = threading.Lock()

    while True:
        lock.acquire()
        canvas.itemconfig(square_1, fill=colors[index - 1])
        time.sleep(5)
        lock.release()


        
if __name__ == '__main__':
    main()
    