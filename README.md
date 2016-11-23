# World-Simulation

Ziting Shen & Qin Yang

1. After pressing 1, 2, 3, you also need to switch to different views to see the boids. Otherwise they might be too small in the default view or not visible in the default view at all.
2. We tried our best to control the boid headlight with the mouse. While it works well for the environment mapped sphere, it does not work exactly correct for the normal phong shader. Actually, it also used to work well in the phong shader, until we changed other parts of code which broke this part. We spent the whole night trying to fix it but still failed. Now the spotlight can be roughly controled by the mouse, but the spotlight is not centered at the cursor location.
3. Loading our nice textures takes some time. Running the program on the mac might slow down the frame rate. We recommend running our program in 231.
4. 