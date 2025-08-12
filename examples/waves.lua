-- Screen center for 128x128
r = 64

function draw()
    cls()
    for y=-r,r,3 do
        for x=-r,r,2 do
            local d=sqrt(x*x+y*y)
            z=cos(d/40-time())*6
            putp(r+x,r+y-z,6)
        end
    end
end