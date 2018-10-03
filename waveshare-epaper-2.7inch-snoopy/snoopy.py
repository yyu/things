import epd2in7
import Image
import ImageDraw
import ImageFont

def getip():
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ipaddr = s.getsockname()[0]
    s.close()
    return ipaddr

def main():
    font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', 14)
    image = Image.open('snoopyonroof.bmp').transpose(Image.ROTATE_270)
    draw = ImageDraw.Draw(image)
    draw.text((150, 164), getip(), font=font, fill=0)

    epd = epd2in7.EPD()
    epd.init()
    epd.display_frame(epd.get_frame_buffer(image.transpose(Image.ROTATE_90)))


if __name__ == '__main__':
    main()
