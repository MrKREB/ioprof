#include "FileWorker.h"


FileWorker::FileWorker()
{
}


FileWorker::~FileWorker()
{
}


void FileWorker::writeFile(char* filename, std::vector<PROCESSINFO> arr) {
	pugi::xml_document doc;
	pugi::xml_node nodeChild;
	pugi::xml_node child;

	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	auto root = doc.append_child("ProfilingSamples");

	for (int i = 0; i < arr.size(); i++) {
		nodeChild = root.append_child();
		nodeChild.set_name("Sample");
		
		child = nodeChild.append_child("pid");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].dwPID).c_str());
		
		child = nodeChild.append_child("ImageName");
		child.append_child(pugi::node_pcdata).set_value(arr[i].ImageName.c_str());

		child = nodeChild.append_child("AllOps");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].AllOpsCnt).c_str());

		child = nodeChild.append_child("WriteOps");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.WriteOperationCount).c_str());

		child = nodeChild.append_child("ReadOps");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.ReadOperationCount).c_str());

		child = nodeChild.append_child("OtherOps");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.OtherOperationCount).c_str());

		child = nodeChild.append_child("AllBytes");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].AllBytes).c_str());

		child = nodeChild.append_child("WrBytes");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.WriteTransferCount).c_str());

		child = nodeChild.append_child("RdBytes");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.ReadTransferCount).c_str());

		child = nodeChild.append_child("OtherBytes");
		child.append_child(pugi::node_pcdata).set_value(std::to_string(arr[i].ioCounters.OtherTransferCount).c_str());
	}

	bool saveSucceeded = doc.save_file(filename, PUGIXML_TEXT("  "));
}



 ProgCodes FileWorker::readFile(char* filename, std::vector<PROCESSINFO> *spi) {
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(filename,
		pugi::parse_default | pugi::parse_declaration);

	if (!result)
		return unableToOpenFIle;
	
	pugi::xml_node root = doc.document_element();
	pugi::xml_attribute attr;

	for (pugi::xml_node node : root.children("Sample")) {
		PROCESSINFO ppi;
		ppi.dwPID = strtoul(node.child("pid").child_value(), NULL, 10);
		ppi.ImageName = node.child("ImageName").child_value();
		ppi.AllOpsCnt = strtoull(node.child("AllOps").child_value(), NULL, 10);
		ppi.ioCounters.WriteOperationCount = strtoull(node.child("WriteOps").child_value(), NULL, 10);
		ppi.ioCounters.ReadOperationCount = strtoull(node.child("ReadOps").child_value(), NULL, 10);
		ppi.ioCounters.OtherOperationCount = strtoull(node.child("OtherOps").child_value(), NULL, 10);
		ppi.AllBytes = strtoull(node.child("AllBytes").child_value(), NULL, 10);
		ppi.ioCounters.WriteTransferCount = strtoull(node.child("WrBytes").child_value(), NULL, 10);
		ppi.ioCounters.ReadTransferCount = strtoull(node.child("RdBytes").child_value(), NULL, 10);
		ppi.ioCounters.OtherTransferCount = strtoull(node.child("OtherBytes").child_value(), NULL, 10);
		spi->push_back(ppi);
	}

	return success;
}