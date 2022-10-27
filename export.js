const xlsx = require('xlsx');
const path = require('path');

const excel = (patients, columns, name_of_wksheet, path_of_file) => {
    const wkbook = xlsx.utils.book_new();
    const data = [
        columns,
        ...patients

    ];
    const wksheet = xlsx.utils.aoa_to_sheet(data);
    xlsx.utils.book_append_sheet(wkbook, wksheet, name_of_wksheet);
    xlsx.writeFile(wkbook, path.resolve(path_of_file));
}

const excelexport = (users, columns, name_of_wksheet, path_of_file) => {
    const patients = users.map(user => {
        return [user.id, user.name, user.number, user.address, user.condition];
    });
    excel(patients, columns, name_of_wksheet, path_of_file);
}

module.exports = excelexport;
