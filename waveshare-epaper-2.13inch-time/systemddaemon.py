import epd2in13
import time
import Image
import ImageDraw
import ImageFont

white = 255

def getip():
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ipaddr = s.getsockname()[0]
    s.close()
    return ipaddr

def display_text(epd, txt):
    font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', 16)

    img_width, img_height = font.getsize(txt)
    img_height = 30 # sometimes the height doesn't work, this is a dirty hack

    img = Image.new('1', (img_width, img_height), white)
    x_limit, y_limit = img_width - 1, img_height - 1

    draw = ImageDraw.Draw(img)

    draw.rectangle((0, 0, img_width, img_height), fill = white) # clear the image
    #draw.line((0, 0, image_x_limit, 0), fill = 0)                        # -
    #draw.line((image_x_limit, 0, 0, image_y_limit), fill = 0)            # /
    #draw.line((0, 0, image_x_limit, image_y_limit), fill = 0)            # \
    #draw.line((0, image_y_limit, image_x_limit, image_y_limit), fill = 0) # _
    draw.text((0, 0), txt, font = font, fill = 0)

    for _ in range(2):
        epd.set_frame_memory(img.transpose(Image.ROTATE_90), 50, (epd2in13.EPD_HEIGHT - img_width) / 2)
        epd.display_frame()


def display_ipaddr(epd):
    font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', 20)

    ipaddr = getip()
    image_width, image_height = font.getsize(ipaddr)
    image_height = 20 # sometimes the height doesn't work, this is a dirty hack

    print(ipaddr, image_width, image_height)

    ip_image = Image.new('1', (image_width, image_height), white)
    image_x_limit, image_y_limit = image_width - 1, image_height - 1

    draw = ImageDraw.Draw(ip_image)

    draw.rectangle((0, 0, image_width, image_height), fill = white) # clear the image
    #draw.line((0, 0, image_x_limit, 0), fill = 0)                        # -
    #draw.line((image_x_limit, 0, 0, image_y_limit), fill = 0)            # /
    #draw.line((0, 0, image_x_limit, image_y_limit), fill = 0)            # \
    #draw.line((0, image_y_limit, image_x_limit, image_y_limit), fill = 0) # _
    draw.text((0, 0), ipaddr, font = font, fill = 0)

    for _ in range(2):
        epd.set_frame_memory(ip_image.transpose(Image.ROTATE_90), 80, (epd2in13.EPD_HEIGHT - image_width) / 2)
        epd.display_frame()


def display_time(epd):
    font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', 32)
    char_width = 19
    char_height = 32
    char_count = 8
    white = 255

    time_image = Image.new('1', (char_width * char_count, char_height), white)  # 255: clear the frame
    image_width, image_height  = time_image.size
    image_h_limit = image_width - 1
    image_v_limit = image_height - 1

    draw = ImageDraw.Draw(time_image)

    while (True):
        # draw a rectangle to clear the image
        draw.rectangle((0, 0, image_width, image_height), fill = white)

        #draw.line((0, 0, image_h_limit, 0), fill = 0)                        # -
        #draw.line((image_h_limit, 0, 0, image_v_limit), fill = 0)            # /
        #draw.line((0, 0, image_h_limit, image_v_limit), fill = 0)            # \
        draw.line((0, image_v_limit, image_h_limit, image_v_limit), fill = 0) # _

        draw.text((0, 0), time.strftime('%H:%M:%S'), font = font, fill = 0)

        epd.set_frame_memory(time_image.transpose(Image.ROTATE_90), 10, (epd2in13.EPD_HEIGHT - image_width) / 2)
        epd.display_frame()

def main():
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)

    image = Image.new('1', (epd2in13.EPD_WIDTH, epd2in13.EPD_HEIGHT), 255)  # 255: clear the frame
    epd.clear_frame_memory(0xFF)
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()

    ###epd.delay_ms(2000)

    #### for partial update
    epd.init(epd.lut_partial_update)
    ###image = Image.open('/home/pi/___/RaspberryPi_3rdParty/ePaperHat2.13inch/raspberrypi/python/monocolor.bmp')

    #####
    #### there are 2 memory areas embedded in the e-paper display
    #### and once the display is refreshed, the memory area will be auto-toggled,
    #### i.e. the next action of SetFrameMemory will set the other memory area
    #### therefore you have to set the frame memory twice.
    #####     
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()

    display_ipaddr(epd)

    #display_text(epd, 'i love you')

    display_time(epd)


if __name__ == '__main__':
    main()
