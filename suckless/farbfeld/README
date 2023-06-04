
                 ███ ███ ██  ██  ███ ███ █   ██
                 █   █ █ █ █ █ █ █   █   █   █ █
                 ██  ███ ██  ███ ██  ██  █   █ █
                 █   █ █ █ █ █ █ █   █   █   █ █
                 █   █ █ █ █ ██  █   ███ ███ ██


WHAT IS FARBFELD?
	Farbfeld is a lossless image-format designed to be
	parsed and piped easily. It is probably the simplest
	image-format you can find (see FORMAT).
	It does not have integrated compression, but allows
	compression algorithms to work with it easily by adding
	little entropy to the image data itself. This beats PNG
	in many cases.
	Given the free choice of compression algorithms, it
	is trivial to switch to better and faster ones as they
	show up in the future.

HOW DO I USE THE TOOLS?
	encoding:
	png2ff < example.png > example.ff
	png2ff < example.png | bzip2 > example.ff.bz2

	decoding:
	ff2png < example.ff > example.png
	bzcat example.ff.bz2 | ff2png > example.png

	bzip2 is used in this example and a recommended
	compression algorithm. Of course you are free
	to use something else.

WHY FARBFELD?
	Current image-formats have integrated compression,
	making it complicated to read the image data.
	One is forced to use complex libraries like libpng,
	libjpeg, libjpeg-turbo, giflib and others, read the
	documentation and write a lot of boilerplate in order
	to get started.
	Farbfeld leaves this behind and is designed to be as
	simple as possible, leaving the task of compression
	to outside tools.
	The simple design, which was the primary objective,
	implicitly lead to the very good compression
	characteristics, as it often happens when you go with
	the UNIX philosophy.
	Reading farbfeld images doesn't require any special
	libraries. The tools in this folder are just a toolbox
	to make it easy to convert between common image formats
	and farbfeld.

HOW DOES IT WORK?
	In farbfeld, pattern resolution is not done while
	converting, but while compressing the image.
	For example, farbfeld always stores the alpha-channel,
	even if the image doesn't have alpha-variation.
	This may sound like a big waste at first, but as
	soon as you compress an image of this kind, the
	compression-algorithm (e.g. bzip2) recognizes the
	pattern that every 48 bits the 16 bits store the
	same information.
	And the compression-algorithms get better and better
	at this.
	Same applies to the idea of having 16 bits per channel.
	It sounds excessive, but if you for instance only have
	a greyscale image, the R, G and B channels will store
	the same value, which is recognized by the compression
	algorithm easily.
	This effectively leads to filesizes you'd normally only
	reach with paletted images, and in some cases bzip2 even
	beats png's compression, for instance when you're dealing
	with grayscale data, line drawings, decals and even
	photographs.
