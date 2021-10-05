#ifndef Route_hpp
#define Route_hpp

# include <string>
# include <iostream>
# include <vector>
# include <sstream>
# include <iterator>
# include <cstring>

namespace WS
{
	class Route
	{
		private:
			std::string 				m_condition;
			std::string 				m_cgi_path;
			std::string 				m_cgi_extension;
			std::vector<std::string>	m_methods;
			std::string 				m_redirection;
			std::string 				m_root;
			std::string 				m_auto_index;
			std::string					m_index;
			std::string 				m_upload;

		public:
			//CONSTRUCTEURS
			Route();

			void 		init(void);
			void 		print(void);
			std::string	isValid();

			//SETTEURS
			void setCondition(std::string str);
			void setCgiPath(std::string str);
			void setCgiExtension(std::string str);
			void setMethods(std::string str);
			void setRedirection(std::string str);
			void setRoot(std::string str);
			void setAutoIndex(std::string str);
			void setIndex(std::string str);
			void setUpload(std::string str);

			//ACCESSEURS
			std::string 				getCgiPath();
			std::string					getCorrectedCgiPath();
			std::string 				getCgiExtension();
			std::string 				getRedirection();
			std::string					getRootDir();
			std::string 				getCondition();
			std::string 				getExistingCondition();
			std::string 				getIndex();
			std::string 				getAutoIndex();
			std::string 				getUpload();
			std::vector<std::string> 	*getMethods();
	};
}

#endif