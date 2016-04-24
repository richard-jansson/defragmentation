# defragmentation
So after a discussion with a guy on reddit I started on working a little on a project to create a defrag program for linux. It would work for only basic formats of FAT.

# Setting up the environment
Allright so the first step is to create a disk image to play with. I haven't read up on the FAT filesystem but I have some general understanding of how it's built up. It should be quite easy to find good resources that explains this!
Allright the first tool in our toolbox for accomplishing this would be dd. So first in order to create a disk image we would need to have a empty file of a certain size. To do this we read from /dev/zero (A nice thing about unix structure is that every device can be accessed like a file). Zero is a device that gives you zeros. DD is like copy but with some more options.
dd if=/dev/zero bs=1M count=32 of=image
Now we have an empty 32 megabyte file which we can use as our "harddisk" this is nice because we don't have to run our program as root, and potentially fuck up our harddrives. (This running you'r own programs as root is really dangerous as I can testify from several programs running amok and destroying installations). In order to put a filesystem on it we use the mkfs.vfat command. I noticed you'd wanted to use 8 bit fat but I mkfs.vfat doesn't support it so I'll go with 16 bit instead. Also I specify only one file allocation table for now since I guess this will be easier to work with. And then I'll use 512 bit sectors. So we'll end up with (216)*512 bytes = 32 megabytes.
mkfs.vfat -F 16 -f 1 -S 512 image
Allright so now we want to write to the file right and how do we do this. Yes we mount it as if it was an regular hard drive. Think of this as daemon tools but built into the standard installation. Here there are some special values 1000 and 1000 is the userid and group id for my account. Probably it's the same for you but to find out for sure. Do grep yourusername /etc/password and find out.
mkdir mount sudo mount -o loop,uid=1000,gid=1000,umask=022,rw image mount
So now we have our disk created in the directory mount. So we can go there and manipulate it any way we want. Standard commands like df, cd and everything works just as they use to!
If you want to information about any program I'm using simply type: man dd or man mount or whatever. All the information you need is in there!
Now the next step for you would be to go and create some fragmentation!

# creating defragmentation
Hm this takes some ingenuity. I only have the time for a quick message today. But tomorrow I can write a more elaborate way. Basically what you want to do is to is to copy back and forth files with different file sizes. So that the filesystem will become defragmentated.
So our filesystem is 30 megabyte big right? So what we do is that we create 30 1mb files A1,B1,A2,B2 etc. Lets say that the As are filled with A and B file is filled with B. Now the disk is filled with files. Now let's remove half of them. Lets say we remove the B files. (We want only one character in the files then we can easily spot the different files in the filesystem with a hex editor). Next step would be to create a file which is 16 mb. Now there won't be one nice long run where this file can be stored. So if we fill this file with Cs. We would have A1, C1, A2, C1, A3, C1, etc. Not necessarily in this order, but somewhere along the lines of this.
Then we can simply spin up the hex editor and watch what happens in the filesystem. The files ought to be split into 512 byte blocks so 1Mb is a bad example. Using 512byte files for A and B would be better, this doesn't affect the size of the C file however.
I guess that creating files at random also would work with random sizes and so forth. But this seems to be the simplest way to create defragmentation that I could think of. Perhaps you have some other idea?
I'll get back to you tomorrow. Then I should have some time to figure out how to write a script that does this. This is the way I prefer to create data. Since you can repeat your tests in a systematic manner. Further down the line when we're developing the actual script. We will need to be able to create defragmentation in a consistent manner. This because usually you work in a change code, test, change code cycle. Not having to create a defragmentation manually would be god send then.

# Program is written

So I've written two c programs and one bash script that you can use to create defragmentation. Create the image as earlier using dd. Then you mount the image as a loop device using a command I've written about earlier.
create_fragmentation.c which creates defragmentation using an algorithm mentioned earlier.
Look in the sourcecode about how to run create_fragmentation.c. Replace frag.c with whatever to make it work.
Frag.sh Then we have a script to use this command to create more files. The c program really is a helper thingy.
And last but not least we have analyse.c
compile this with
gcc analyce.c -o analyze -lm run it using this command: ./analyze | less
So there you see the count of different letters and you'll see we have around the same values for A as for B. And if you open the image file that contains the FAT drive we are using in a hex-editor. Such as ghex and scroll in it. We will see As interlaced with Cs. Q.E.D. we have constructed defragmentation.
