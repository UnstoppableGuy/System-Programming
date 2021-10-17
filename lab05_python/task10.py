from kivy.app import App
from kivy.core.window import Window
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.graphics import Line, Rectangle, Triangle, Ellipse, Color

Window.size = (700, 500)
Window.title = 'Task10'


def color():
    return lambda: None


def figure(x):
    return lambda x: x


class MainScreen(Screen):
    def draw_figure(self):
        global color
        global figure

        if self.draw_checkbox.active:

            def new_color():
                if self.red_checkbox.active:
                    return lambda: Color(1, 0, 0)
                elif self.green_checkbox.active:
                    return lambda: Color(0, 1, 0)
                else:
                    return lambda: Color(0, 0, 1)

            def new_figure(x):
                if self.rhombus_checkbox.active:
                    return lambda x: Line(points=[
                        x[0], x[1],
                        x[0]-50, x[1]+50,
                        x[0], x[1]+100,
                        x[0]+50, x[1]+50,
                        x[0], x[1]])
                elif self.square_checkbox.active:
                    return lambda x: Rectangle(pos=x, size=[100, 100])
                elif self.rectangle_checkbox.active:
                    return lambda x: Rectangle(pos=x, size=[150, 50])
                else:
                    return lambda x: Line(points=[
                        x[0], x[1],
                        x[0]-40, x[1]-100,
                        x[0]+60, x[1]-40,
                        x[0]-60, x[1]-40,
                        x[0]+40, x[1]-100,
                        x[0], x[1],
                        ])

            color = new_color()

        else:
            def new_figure(x):
                return lambda x: x

        figure = new_figure(lambda x: x)


class CanvasScreen(Screen):
    def on_touch_down(self, touch):
        global color
        global figure

        with self.canvas:
            color()
            figure(touch.pos)

        super().on_touch_down(touch)


class ScreenManagement(ScreenManager):
    pass


scr = Builder.load_file('task10.kv')


class Task10App(App):
    def build(self):
        return scr


Task10App().run()
