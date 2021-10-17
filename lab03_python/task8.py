from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.core.window import Window
from kivy.graphics import Line, Rectangle, Triangle, Ellipse, Color

import random

Window.size = (700, 500)
Window.title = 'Task8'


class Container(FloatLayout):

    def draw_figure(self):
        x1, y1 = self.float_layout.pos
        x2, y2 = self.float_layout.size
        x2 += x1
        y2 += y1

        with self.canvas:
            #First house
            Color(0.92, 0.71, 0.36)
            Rectangle(pos=[x1 + 100, y1 + 5], size=[100, 100])

            Color(0.23, 0.15, 0)
            Rectangle(pos=[x1 + 180, y1 + 105], size=[10, 100])

            Color(0.11, 0.12, 0.84)
            Triangle(points=[
                x1 + 80, y1 + 105,
                x1 + 220, y1 + 105,
                x1 + 150, y1 + 200])

            #Secpnd house
            Color(0.92, 0.71, 0.36)
            Rectangle(pos=[x1 + 100+150, y1 + 5], size=[100, 100])

            Color(0.23, 0.15, 0)
            Rectangle(pos=[x1 + 180+150, y1 + 105], size=[10, 100])

            Color(1, 0, 0)
            Triangle(points=[
                x1 + 80+150, y1 + 105,
                x1 + 220+150, y1 + 105,
                x1 + 150+150, y1 + 200])    

            Color(1, 1, 0)
            Ellipse(pos=[x1 + 400, y2 - 100])

            Color(0.10, 0.36, 0)

            for i in range(int(x2 - x1)):
                Line(points=[
                    x1 + i, y1,
                    x1 + random.randint(-10, 10) + i,
                    y1 + random.randint(6, 19)])

    def clear_canvas(self):
        with self.canvas:
            Color(1, 1, 1)
            Rectangle(pos=self.float_layout.pos, size=self.float_layout.size)


class Task8App(App):
    def build(self):

        return Container()


Task8App().run()
