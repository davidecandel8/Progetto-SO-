public class Merce {

    int indice_merce; //indice della merce 
    int SO_VITA; //data di scadenza
    int quantità; //quantita di merce per lotto (in ton)

    public Merce(int n) {
        indice_merce = n;
        SO_VITA = (int) Consegna.Random((Consegna.SO_MAX_VITA - Consegna.SO_MIN_VITA) + 1) + Consegna.SO_MIN_VITA;
        quantità = (int)Consegna.Random(Consegna.SO_SIZE) + 1;
    }

    public String toString(){
        return "La merce tipo [" + indice_merce + "]: \n   # giorni di vita della merce: " + SO_VITA + "\n   # quantità di merce per lotto (in ton): " + quantità; 
    }

}
