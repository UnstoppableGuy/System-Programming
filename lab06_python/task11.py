from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.core.window import Window
from kivymd.theming import ThemeManager
from kivy.animation import Animation

from threading import Thread


Window.size = (700, 500)
Window.title = 'Task11'


class Container(FloatLayout):
    anim_1 = Animation(pos_hint={'center_x': 0, 'y': 0.2}, duration=2)
    anim_2 = Animation(pos_hint={'center_x': 0.5, 'y': 0.8}, duration=1)
    anim_3 = Animation(pos_hint={'center_x': 0.1, 'y': 0.1}, duration=5)

    def mov_text_1(self):
        self.anim_1.start(self.text_label_1)

    def mov_text_2(self):
        self.anim_2.start(self.text_label_2)

    def mov_text_3(self):
        self.anim_3.start(self.text_label_3)

    def start_moving(self):
        self.start_button.disabled = True
        self.stop_button.disabled = False

        self.anim_1.repeat = True
        self.anim_2.repeat = True
        self.anim_3.repeat = True

        th_1 = Thread(target=self.mov_text_1)
        th_2 = Thread(target=self.mov_text_2)
        th_3 = Thread(target=self.mov_text_3)

        th_1.start()
        th_2.start()
        th_3.start()

    def stop_moving(self):
        self.start_button.disabled = False
        self.stop_button.disabled = True

        self.anim_1.repeat = False
        self.anim_1.cancel(self.text_label_1)

        self.anim_2.repeat = False
        self.anim_2.cancel(self.text_label_2)

        self.anim_3.repeat = False
        self.anim_3.cancel(self.text_label_3)


class Task11App(App):
    theme_cls = ThemeManager()

    def build(self):
        self.theme_cls.theme_style = 'Light'
        return Container()


Task11App().run()
