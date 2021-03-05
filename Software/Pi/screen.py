import sys, pygame
pygame.init()

size = width, height = 800, 480
speed = [2, 2]

bgColour = pygame.Color(0, 0, 0)
white = pygame.Color(255, 255, 255)

screen = pygame.display.set_mode(size,pygame.FULLSCREEN)
square = pygame.Surface((5,5))
square.fill(white)
squareRect = square.get_rect(topleft=(20,20))

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    squareRect = squareRect.move(speed)
    if squareRect.left < 0 or squareRect.right > width:
        speed[0] = -speed[0]
    if squareRect.top < 0 or squareRect.bottom > height:
        speed[1] = -speed[1]

    screen.fill(bgColour)
    screen.blit(square, squareRect)
    pygame.display.flip()
