# ising

ising is a simple [Ising Model](https://en.wikipedia.org/wiki/Ising_model) implementation in c. The Hamiltonian includes the nearest neighbors and external magnetic field spin interactions. There is a third term that includes a temporal dependence, a feedback with arbitrary delay. Not sure if this last term has any physical meaning, but i got inspired by some papers from the 80's about Ising models of neural networks...

## Requirements

A machine with gcc, [libpng](http://www.libpng.org/pub/png/libpng.html).

[ffmpeg](https://ffmpeg.org/) for video generation.

## Compilation

Clone the repo

```bash
git clone https://github.com/srivera1/ising.git
```

Compile the code

```bash
make
```

On debian, one liner for quick testing:

```bash
clear; rm output/* ; make clean; make && ./ising -V 123 -H 200 -J -1.2 -j1 -S 100 && eog output/*
```

## Usage

```python
./ising -h
usage: ./ising [-H int] [-J float] [-M float] [-S int]
 [-T int] [-V int] [-c float] [-h] [-j float] [-m float] 
 [-n float] [-r float] [-s float] [-t float] [-w float]
  -H int         horizontal lattice size
  -J float       max J
  -M float       max magnetic moment
  -S int         simulation steps
  -T int         steps of delay for the feedback
  -V int         vertical lattice size
  -c float       temperature cooling factor
  -h             print this message
  -j float       min J
  -m float       min magnetic moment
  -n float       spin coupling parameter - nearest
  -r float       spin coupling parameter - magnetic
  -s float       spin coupling parameter - time
  -t float       temperature
  -w float       temperature warming factor
```

```python
rm output/*
./ising -V 123 -H 200 -J -1.2 -j1 -S 100
```

Check the output images

```bash
eog output/*
```

Make a video with the output

```bash
cd output/
ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4
```

Some results

```bash
rm output/*
rm examples/*
sh examples.sh
```
[Check the generated videos](https://github.com/srivera1/ising/tree/main/examples)



```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 0.0 -s 0.0 -t 293.15 -w 1.0
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e2.png?raw=true)

```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 1.0 -s 0.0 -t 293.15 -w 1.0
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e3.png?raw=true)

```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 7 -V 200 -c 1.0 -j 1.0 -m -1.0 -n 1.0 -r 1.0 -s 0.01 -t 293.15 -w 1.0
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e4.png?raw=true)

```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 1.0 -s 0.10 -t 293.15 -w 1.0
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e5.png?raw=true)

```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m -1.0 -n 1.0 -r 1.0 -s 0.10 -t 293.15 -w 1.0
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e6.png?raw=true)

```bash
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 0.9990 -j 1.0 -m -1.0 -n 1.0 -r 0.10 -s 0.10 -t 293.15 -w 1.00010
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e7.png?raw=true)

```bash
./ising -H 500 -S 1000 -T 20 -V 200
```

![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e8.png?raw=true)

```bash
./ising -H 200 -S 1000 -T 30 -V 500 -t 100.0
```
![alt text](https://raw.githubusercontent.com/srivera1/ising/main/examples/ising_1000_e9.png?raw=true)


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

