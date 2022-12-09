
# compute confidence interval with Student's T distribuition
# before computing anything check if RV are normal and iid
def compute_ci_student_t (data,alpha = 0.95) : 
    # number of degrees of freedom of the sample
    degrees = len(data)-1
    # sample mean
    mean = np.mean(data)
    # std error = sample_std/sqrt(n)    
    sem = st.sem(data)
    return st.t.interval(alpha,degrees,mean,sem)    
