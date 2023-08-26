class Offerta{
    
    int ind_merce; 
    int ind_porto; 
    int lotti; 
    int priorità; 
    
    public Offerta(int merce, int porto, int quantità){
        ind_merce = merce;
        ind_porto = porto; 
        lotti = quantità; 
    }

    public String toString(){
        return "\n  # Porto = " + ind_porto + "\n  # Merce = " + ind_merce + "\n  # Quantità offerta = " + lotti;
    }
}