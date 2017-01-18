import numpy as np
from matplotlib import pyplot as plt

def Homework4_p2(path, data_filenames):
    ### import data ###
    data = np.loadtxt(path+data_filenames+'.txt', delimiter=',', usecols=range(4))
    ### plot results ###
    fig = plt.figure()
    plt.loglog(data[:,0],data[:,2],'r*',markersize=14,label='Estimate error')
    plt.loglog(data[:,0],data[:,3],'b+',markersize=14,label='Actual error')
    plt.loglog(data[:,0], 1/np.sqrt(data[:,0]),'green',label=r'$10 \times N^{-1/2}$, theoretical')
    
    plt.legend()
    plt.xlabel('N')
    plt.ylabel('Errors')
    plt.title('OpenMP ,Actual error and Estimated error')
    plt.savefig(path+data_filenames+'.png' , bbox_inches='tight')
    return

def Homework4_p3c(path, data_filenames, data_filenames_2):
    ### import data ###
    data = np.loadtxt(path+data_filenames+'.stdou', delimiter=',', usecols=range(5))
    data_2 = np.loadtxt(path+data_filenames_2+'.stdou', delimiter=',', usecols=range(5))
    ### Spedup Calculation #############
    # For spedup I used  http://stackoverflow.com/questions/7925510/parallel-speedup-with-openmp
    # equential time measurement: seq_time = endTime-startTime;
    # Parallel time measurement : paralleltime = endTime-startTime; 
    # speedup = seq_time/paralleltime;
    seq_time = data[0,2] 
    new_col = np.zeros((len(data), 1))
    for i,j in  enumerate(data):
        new_col[i] = seq_time / data[i,2]
    data = np.c_[data, new_col]

    seq_time = data_2[0,2] 
    new_col = np.zeros((len(data_2), 1))
    for i,j in  enumerate(data_2):
        new_col[i] = seq_time / data_2[i,2]
    data_2 = np.c_[data_2, new_col]
    
    print(data)

    print(data_2)

    
    ### plot results ###
    fig = plt.figure()
    plt.plot(data[:,1],data[:,5],'r*',markersize=20,label='Strong Scaling Test')
    plt.plot(data_2[:,1],data_2[:,5],'bo',markersize=8,label='Weak Scaling Test')

    
    plt.legend(loc='upper right')
    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.title('OpenMP ,scaling test')
    plt.savefig(path+data_filenames+'.png' , bbox_inches='tight')
    return

def Homework4_p3e(path, data_filenames):
    ### import data ###
    data = np.loadtxt(path+data_filenames+'.stdou', delimiter=',', usecols=range(2))
    ### For Estimated Error Calculation #############
    new_col = np.zeros((len(data), 1))
    for i in range(len(data)-2,-1,-1):
        new_col[i] = abs(data[i,1] - data[i+1,1])
    
    new_col[len(data)-1] = new_col[len(data)-2] 
    data = np.c_[data, new_col]
    ### plot results ###
    fig = plt.figure()
    ax1 = fig.add_subplot(211)
    ax1.semilogx(data[:,0],data[:,1],'r*',markersize=16,label='$\mathcal{L}$ Approximation')
    ax1.legend(loc='upper left')
    ax1.set_ylabel('L')

    ax2 = fig.add_subplot(212)
    ax2.semilogx(data[:,0],data[:,2],'bo',markersize=10,label='Estimate error')
    ax2.legend(loc='upper left')
    ax2.set_ylabel('error')
    ax2.set_xlabel('N')

    fig.suptitle('OpenMP, MC Aprroximation      File:' + data_filenames)
    plt.savefig(path+data_filenames+'.png' , bbox_inches='tight')
    return






def run():    
    path = "/Users/ekinezgi/Documents/UmassD/DSC520/umassd-hpc-mehmetduman/HW4/"
    fnames_p2b = "hw4_p2b"
    fnames_p3c = "hw4_p3c_7822316"
    fnames_p3d = "hw4_p3d_7822321"

    fnames_p3e = "hw4_p3e9_7820263"
#     fnames_p3e = "hw4_p3e9_mycomputer"

    
    Homework4_p2(path,fnames_p2b) 
    Homework4_p3c(path,fnames_p3c, fnames_p3d) 
    Homework4_p3e(path,fnames_p3e) 


    return
    
if __name__ == '__main__':
    run()
