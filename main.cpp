#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Produk {
    int pk;
    char nama [20];
    int harga;
};

int pilihMenu(); //deklarasi fungsi
void checkDatabase(fstream &data); //deklarasi fungsi

void writeData(fstream &data, int posisi, Produk &inputProduk)
{
    data.seekp((posisi-1)*sizeof(Produk), ios::beg); //mencari posisi
    data.write(reinterpret_cast<char*>(&inputProduk), sizeof(Produk));
}
int getDataSize(fstream &data)
{
    int start, end;
    data.seekg(0, ios::beg); //mencari posisi dlm data dan pindah cursor didepan 0
    start = data.tellg();
    data.seekg(0, ios::end);
    end = data.tellg();

    return (end - start)/sizeof(Produk);
}

Produk readData(fstream &data, int posisi)
{
    Produk readProduk;
    data.seekp((posisi-1)*sizeof(Produk), ios::beg); //mencari posisi
    data.read(reinterpret_cast<char*>(&readProduk), sizeof(Produk));
    return readProduk;
}

void addDataProduk(fstream &data)
{
    Produk inputProduk, lastProduk;
    int size;
    size = getDataSize(data);
    if(size == 0){
        inputProduk.pk = 1;
    } else {
        lastProduk = readData(data, size);
        inputProduk.pk = lastProduk.pk + 1;
    }
    cout << "jumlah produk yang sudah diinput" << size << endl;
    cout << "pk sekarang adalah " << inputProduk.pk << endl;

    cout << "Nama Produk : ";
    cin.getline(inputProduk.nama, 20);
    cout << "Harga Produk: ";
    cin >> inputProduk.harga;

    writeData(data, size+1 , inputProduk);
}

void displayDataProduct (fstream &data) {
    Produk showProduct;
    int size;

    size = getDataSize(data);

    for(int i = 1; i <= size; i++) {
        showProduct = readData(data, i);
        std::cout << i << "\t";
        std::cout << showProduct.pk << "\t";
        std::cout << showProduct.nama << "\t";
        std::cout << showProduct.harga << "\t" << std::endl;
    }
}

void updateDataProduct(fstream &data) {
    int nomor;
    Produk updateProduk;

    std::cout << "Masukkan nomor produk yang akan diubah : ";
    std::cin >> nomor;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    updateProduk = readData(data, nomor);

    std::cout << "Detail produk yang akan diubah : " << std::endl;
    std::cout << "Nama produk : " << updateProduk.nama << std::endl;
    std::cout << "Harga produk : " << updateProduk.harga << std::endl;

    std::cout << "Detail produk baru : " << std::endl;
    std::cout << "Nama produk : ";
    std::cin.getline(updateProduk.nama, 20);
    std::cout << "Harga produk : ";
    std::cin >> updateProduk.harga;

    writeData(data, nomor, updateProduk);
}

void deleteDataProduct (fstream &data) {
    int nomor, size, offset;
    Produk blankProduct, temporaryProduct;
    fstream dataSementara;

    size = getDataSize(data);
    std::cout << "Pilih nomor produk yang akan dihapus : ";
    std::cin >> nomor;

    blankProduct.pk = 0;
    writeData(data, nomor, blankProduct);

    dataSementara.open("temp.dat", ios::trunc | ios::out | ios::in | ios::binary);
    offset = 0;
    for(int i = 1; i<=size; i++) {
        temporaryProduct = readData(data, i);
        if (temporaryProduct.pk != 0) {
            writeData(dataSementara, i-offset, temporaryProduct);
        } else {
            offset = offset+1;
        }
    }

    size = getDataSize(dataSementara);
    data.close();
    data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
    data.close();
    data.open("data.bin", ios::out | ios::in | ios::binary);

    for(int i = 1; i <= size; i++) {
        temporaryProduct = readData(dataSementara, i);
        writeData(data, i, temporaryProduct);
    }
}

int main ()
{
    fstream data;
    checkDatabase(data);

    int pilihan = pilihMenu();
    char is_continue;

    while (pilihan != 5)
    {
        switch (pilihan)
        {
            case 1 :
                cout <<  "Menu tambah produk" << endl;
                addDataProduk(data);
                break;
            case 2 :
                cout <<  "Menu lihat produk" << endl;
                displayDataProduct(data);
                break;
            case 3 :
                cout <<  "Menu ubah produk" << endl;
                displayDataProduct(data);
                updateDataProduct(data);
                displayDataProduct(data);
                break;
            case 4 :
                cout <<  "Menu hapus produk" << endl;
                displayDataProduct(data);
                deleteDataProduct(data);
                displayDataProduct(data);
                break;
            default :
                cout << "menu tidak valid" << endl;
                break;
        }

        label_continue:
        cout << "lanjutkan?[y/n] :";
        cin >> is_continue;
        if(is_continue == 'y' | is_continue == 'Y') {
            pilihan = pilihMenu();
        } else if (is_continue == 'n' | is_continue == 'N') {
            break;
        } else {
            goto label_continue;
        }
    }


    return 0;
}

int pilihMenu()
{
    int input;
    cout << "Program CRUD Produk" << endl;
    cout << "====================" << endl;
    cout << "1. Tambah Produk" << endl;
    cout << "2. Lihat Produk" << endl;
    cout << "3. Ubah Produk" << endl;
    cout << "4. Hapus Produk" << endl;
    cout << "5. Selesai" << endl;
    cout << "====================" << endl;
    cout << "Masukkan pilihan? [1-5]:";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;

}

void checkDatabase(fstream &data)
{
    data.open("data.bin", ios::out | ios::in | ios::binary);
    if (data.is_open()) {
        cout << "database ditemukan!" << endl;
    } else {
        cout << "database belum ada, buat baru" << endl;
        data.close ();
        data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
    }

}