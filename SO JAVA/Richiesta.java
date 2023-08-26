class Richiesta{

    int ind_merce; 
    int ind_porto; 
    int lotti; 
    int priorità;

    public Richiesta(int merce, int porto, int quantità){
        ind_merce = merce;
        ind_porto = porto; 
        lotti = quantità;
        priorità = 0;  
    }

    public String toString(){
        return "\n  # Porto = " + ind_porto + "\n  # Merce = " + ind_merce + "\n  # Quantità richiesta = " + lotti;
    }
}