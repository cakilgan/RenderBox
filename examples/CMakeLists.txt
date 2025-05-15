# examples/CMakeLists.txt içinden:
file(GLOB EXAMPLES    "*.cpp"
                       )
# Örnekler için ayrı ayrı hedefler oluştur
foreach(example_file ${EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)  # Dosya adını al

    # Hedefi ekle
    add_executable(${example_name} ${example_file}
     )

    # Bağlantı kütüphanelerini ekle
    target_link_libraries(${example_name} glfw glad glm core)

    # Include dizinlerini ekle
    target_include_directories(${example_name} PRIVATE
        ${GLM_DIR}
        ${GLFW_DIR}/include
        ${GLAD_DIR}
        ${SOURCE_DIR}
    )
endforeach()


