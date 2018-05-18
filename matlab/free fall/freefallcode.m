h = 5000;
stp = 31.9438; %end time (s)
v = 0; %init_v0
k = 0; %init_counter
g = 9.81; %g (m/s^2)
%% start simulation
for t = 0:1:32
    if t<=stp % stop judgement
        y = h - 0.5*g*t^2; %calculate the hight
        scatter(0,y,70,'o','k'); %draw the current position
        title('free fall');
        ylabel('height');
        axis([-1,1,0,5000]); %define the y-axis
        pause(.01); %pause 0.01s
    else
        stp; % output time
        y; %output falling height
        v = g*stp; %output v
    end
end

 
